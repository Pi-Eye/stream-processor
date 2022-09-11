#ifndef STREAM_PROCESSOR_HPP
#define STREAM_PROCESSOR_HPP

#include <ostream>

#include "jpeg_compressor.hpp"
#include "jpeg_decompressor.hpp"
#include "motion_detector.hpp"
#include "text_overlay.hpp"

/**
 * Processed - struct for proccseed frames
 *
 * compressed:  Compressed JPEG frame
 * motion:      Motion of frame or not
 */
struct Processed {
  Compressed compressed;
  bool motion;
};

class StreamProcessor {
 public:
  /**
   * StreamProcessor() - Constructor for stream processor
   */
  StreamProcessor() = default;

  /**
   * SetRequired() - Sets required settings (required)
   *
   * width:             Width of stream
   * height:            Height of stream
   * frame_format:      Format for output (RGB or Grayscale)
   * quality:           Quality for output (JPEG compression quality)
   */
  void SetRequired(unsigned int width, unsigned int height, CompFrameFormat frame_format, int quality);

  /**
   * SetFont() - Sets font settings for overlay (optional)
   *
   * font_settings:     Setting for text overlay
   */
  void SetFont(FontSettings font_settings);

  /**
   * SetMotionFPSScale() - Sets rate scale for motion detection (optional)
   *
   * motion_fps_scale:  Denominator of multiple of fps to run motion detection at (i.e. motion_fps_scale = 2, fps = 30, motion_fps_ = 15)
   */
  void SetMotionFPSScale(unsigned int motion_fps_scale);

  /**
   * SetMotionSettings() - Sets settings for motion detection (optional)
   *
   * motion_config:     Motion detection configuration
   * device_config:     Motion detection device configuration
   */
  void SetMotionSettings(MotionConfig motion_config, DeviceConfig device_config);

  /**
   * SetOutput() - Sets output for info messages (optional)
   *
   * output:    Output stream of info messages
   */
  void SetOutput(std::ostream* output);

  /**
   * ~StreamProcessor() - Deconstructor for Stream Processor
   */
  ~StreamProcessor();

  /**
   * ProcessFrame() - Processes a frame
   *
   * compressed_image:    Compressed frame
   * jpeg_size:           Buffer size of jpeg
   * timestamp:           Timestamp of frame
   * returns:   Processed - Processed frame
   */
  Processed ProcessFrame(const unsigned char* compressed_image, unsigned long jpeg_size, long long timestamp = 0);

 private:
  bool text_;                    // Overlay text or not
  bool motion_;                  // Detect motion or not
  bool last_motion_ = false;     // If motion detected on last frame or not
  unsigned int motion_fps_ = 1;  // Multiple of fps to run motion detection at
  unsigned int frame_count_;     // Frames processed (modulus (fps/motion_fps_))

  InputVideoSettings vid_settings_;  // Video settings

  JpegDecompressor* jpeg_decompressor_ = nullptr;  // Jpeg decompressor
  JpegCompressor* jpeg_compressor_ = nullptr;      // Jpeg compressor
  MotionDetector* motion_detector_ = nullptr;      // Motion detector
  TextOverlay* text_overlay_ = nullptr;            // Text overlayer

  std::ostream* info_ = new std::ostream(0);  // Info output stream
};

#endif