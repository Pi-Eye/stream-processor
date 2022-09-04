#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <raspicam/raspicam.h>

#include "motion_detector.hpp"

/**
 * CameraSettings - Settings for camera
 *
 * fps:                 fps
 * brightness:          brightness [0, 100]
 * sharpness:           sharpness [-100, 100]
 * contrast:            contrast [-100, 100]
 * saturation:          saturation [-100, 100]
 * iso:                 camera ISO [100, 800]
 * exposure_comp:       exposure compensation [-10, 10]
 * horizontal_flip:     flip frame horizontally or not
 * vertical_flip:       flip frame vertically or not
 * awb:                 type of white balance to use
 * exposure_mode:       type of exposure mode to use
 */
struct CameraSettings {
  unsigned int fps;
  unsigned int brightness;
  int sharpness;
  int contrast;
  int saturation;
  unsigned int iso;
  int exposure_comp;
  bool horizontal_flip;
  bool vertical_flip;
  RASPICAM_AWB awb;
  RASPICAM_EXPOSURE exposure_mode;
};

/**
 * Frame - Struct to hold frame information together
 *
 * data:        Actual data of frame
 * timestamp:   Unix timestamp of time frame was taken
 */
struct Frame {
  unsigned char* data;
  long long timestamp;
};

/**
 * Camera - Class for interfacing with raspberry pi camera
 */
class Camera {
 public:
  /**
   * Camera() - Constructor for Camera class
   *
   * video_settings:   InputVideoSettings of frames to take
   */
  Camera(InputVideoSettings video_settings, CameraSettings cam_settings);

  /**
   * GetFrame() - Takes a frame
   *
   * returns:   Frame - info for the frame taken
   */
  Frame GetFrame();
};

#endif