#include "camera.hpp"

#include <raspicam/raspicam_still.h>
#include <chrono>
#include <stdexcept>

#include "motion_detector.hpp"

Camera::Camera(InputVideoSettings video_settings, CameraSettings cam_settings) {
  // Check settings
  ValidateSettings(video_settings, cam_settings);

  // Start camera
  camera_ = raspicam::RaspiCam_Still();

  // Set camera settings
  camera_.setFormat(format_);
  camera_.setCaptureSize(vid_settings_.width, vid_settings_.height);
  camera_.setBrightness(cam_settings.brightness);
  camera_.setSharpness(cam_settings.sharpness);
  camera_.setContrast(cam_settings.contrast);
  camera_.setSaturation(cam_settings.saturation);
  camera_.setISO(cam_settings.iso);
  camera_.setExposureCompensation(cam_settings.exposure_comp);
  camera_.setExposure(cam_settings.exposure_mode);
  camera_.setHorizontalFlip(cam_settings.horizontal_flip);
  camera_.setVerticalFlip(cam_settings.vertical_flip);

  camera_.setEncoding(raspicam::RASPICAM_ENCODING::RASPICAM_ENCODING_JPEG);  // Take images in jpeg format

  if (!camera_.open()) throw std::runtime_error("Failed to open camera");
  usleep(3000000);  // NOLINT(readability-magic-numbers)
}

Frame Camera::GetFrame() {
  size_t size = camera_.getImageBufferSize() unsigned char* data = new unsigned char[size];
  Camera.grab_retrieve(data, size);
  long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()).const();
  return {data, timestamp};
}

void Camera::ValidateSettings(InputVideoSettings video_settings, CameraSettings cam_settings) const {
  // NOLINTBEGIN(readability-magic-numbers)
  if (vid_settings.width == 0) throw std::invalid_argument("Width cannot be 0");
  if (vid_settings.height == 0) throw std::invalid_argument("Height cannot be 0");

  if (cam_settings.fps == 0) throw std::invalid_argument("FPS cannot be 0");

  if (cam_settings.brightness > 100) throw std::invalid_argument("Brightness cannot be more than 100");

  if (cam_settings.sharpness < -100) throw std::invalid_argument("Sharpness cannot be less than -100");
  if (cam_settings.sharpness > 100) throw std::invalid_argument("Sharpness cannot be more than 100");

  if (cam_settings.contrast < -100) throw std::invalid_argument("Contrast cannot be less than -100");
  if (cam_settings.contrast > 100) throw std::invalid_argument("Contrast cannot be more than 100");

  if (cam_settings.saturation < -100) throw std::invalid_argument("Saturation cannot be less than -100");
  if (cam_settings.saturation > 100) throw std::invalid_argument("Saturation cannot be more than 100");

  if (cam_settings.iso < 100) throw std::invalid_argument("ISO cannot be less than 100");
  if (cam_settings.iso > 800) throw std::invalid_argument("ISO cannot be more than 800");

  if (cam_settings.exposure_comp < -10) throw std::invalid_argument("Exposure compenstation cannot be less than -10");
  if (cam_settings.exposure_comp > 10) throw std::invalid_argument("Exposure compenstation cannot be more than 10");
  // NOLINTEND(readability-magic-numbers)
}