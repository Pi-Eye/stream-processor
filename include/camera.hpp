#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "motion_detector.hpp"

/**
 * Frame - Struct to hold frame information together
 *
 * data:        Actual data of frame
 * timestamp:   Unix timestamp of time frame was taken
 */
struct Frame {
  unsigned char* data;
  unsigned int timestamp;
};

/**
 * Camera - Class for interfacing with raspberry pi camera
 */
class Camera {
  /**
   * Camera() - Constructor for Camera class
   *
   * video_settings:   InputVideoSettings of frames to take
   */
  Camera(InputVideoSettings video_settings);

  /**
   * GetFrame() - Takes a frame
   *
   * returns:   Frame - info for the frame taken
   */
  Frame GetFrame();
};

#endif