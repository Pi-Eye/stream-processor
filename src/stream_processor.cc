#include "stream_processor.hpp"

#include <iostream>
#include <map>

#include "jpeg_compressor.hpp"
#include "jpeg_decompressor.hpp"
#include "motion_detector.hpp"
#include "text_overlay.hpp"

void StreamProcessor::SetRequired(unsigned int width, unsigned int height, CompFrameFormat frame_format, int quality) {
  // Convert CompFrameFormat to DecompFrameFormat
  std::map<CompFrameFormat, DecompFrameFormat> decomp_format = {{CompFrameFormat::kGray, DecompFrameFormat::kGray}, {CompFrameFormat::kRGB, DecompFrameFormat::kRGB}};
  vid_settings_ = {width, height, decomp_format.at(frame_format)};

  // Setup compressors and decompressors
  jpeg_compressor_ = new JpegCompressor(static_cast<int>(width), static_cast<int>(height), quality, frame_format, CompFrameMethod::kFast);
  jpeg_decompressor_ = new JpegDecompressor(width, height, vid_settings_.frame_format, DecompFrameMethod::kFast);
}

void StreamProcessor::SetFont(FontSettings font_settings) {
  text_ = true;
  text_overlay_ = new TextOverlay(vid_settings_, font_settings);  // NOLINT(performance-unnecessary-value-param)
}

void StreamProcessor::SetMotionFPSScale(unsigned int motion_fps_scale) { motion_fps_ = motion_fps_scale; }

void StreamProcessor::SetMotionSettings(MotionConfig motion_config, DeviceConfig device_config) {
  motion_ = true;
  motion_detector_ = new MotionDetector(vid_settings_, motion_config, device_config, info_);
}

void StreamProcessor::SetOutput(std::ostream* output) { info_ = output; }

StreamProcessor::~StreamProcessor() {
  delete motion_detector_;
  delete text_overlay_;
  delete jpeg_compressor_;
  delete jpeg_decompressor_;
}

Processed StreamProcessor::ProcessFrame(unsigned char* compressed_image, unsigned long jpeg_size, long long timestamp) {
  // Decompress image
  unsigned char* decompressed = jpeg_decompressor_->DecompressImage(compressed_image, jpeg_size);

  // Run motion detection at correct fps if motion is enabled
  if (motion_) {
    frame_count_++;
    if (frame_count_ == motion_fps_) {
      frame_count_ = 0;
      last_motion_ = motion_detector_->DetectOnDecompressedFrame(decompressed);
    }
  }

  // Run text overlay if enabled
  if (text_overlay_) text_overlay_->OverlayOnFrame(decompressed, timestamp);

  // Recompress image
  Compressed compressed = jpeg_compressor_->CompressImage(decompressed);
  return {compressed, last_motion_};
}
