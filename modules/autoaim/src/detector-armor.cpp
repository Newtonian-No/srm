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
  if (!armor_detector_->Run(image, armor_list_)) {
    LOG(ERROR) << "Failed to detect armor.";
    return false;
  }
  if (!armor_list_.empty()) {
    yaw_ = 0;
    pitch_ = 0;
  } else {
    drawer_->DrawArmor(armor_list.front());
    const Armor& the_one = *armor_list.front();
    if (the_one.color != color_) {
      cv::Point2f center = the_one.Center();
    }
  }
  return true;
}

}  // namespace srm::autoaim