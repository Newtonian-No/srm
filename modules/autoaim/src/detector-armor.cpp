#include "srm/autoaim/detector-armor.h"

namespace srm::autoaim {

bool ArmorDetector::Initialize() {
  /// 初始化yolo
#if defined(__APPLE__)
  std::string net_type = "coreml";
#elif defined(__linux__)
  std::string net_type = "tensorrt";
#endif
  yolo_.reset(nn::CreateYolo(net_type));
  std::string prefix = "nn.yolo.armor";
  const auto model_path = cfg.Get<std::string>({prefix, net_type});
  const auto class_num = cfg.Get<int>({prefix, "class_num"});
  const auto point_num = cfg.Get<int>({prefix, "point_num"});
  if (!yolo_->Initialize(model_path, class_num, point_num)) {
    LOG(ERROR) << "Failed to load armor nerual network.";
    return false;
  }
  return true;
}

bool ArmorDetector::Run(cv::Mat REF_IN image, ArmorPtrList REF_OUT armor_list) const {
  /// 请补全
  const auto objs = yolo_->Run(image);
  for (auto& [x1, y1, x2, y2, prob, clas, pts] : objs) {
    Armor armor{pts[0], pts[1], pts[2], pts[3], static_cast<Color>(clas)};
    armor_list.push_back(std::make_shared<Armor>(std::move(armor)));
  }
  return true;
}

}  // namespace srm::autoaim