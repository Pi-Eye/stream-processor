
#include "jpeg_compressor.hpp"

#include <turbojpeg.h>

#include <map>
#include <stdexcept>

JpegCompressor::JpegCompressor(int width, int height, int quality, CompFrameFormat frame_format, CompFrameMethod comp_method)
    : width_(width), height_(height), quality_(quality) {
  if (quality == 0) throw std::invalid_argument("JPEG quality cannot be equal to 0");
  if (quality > 100) throw std::invalid_argument("JPEG quality cannot be greater than 100");  // NOLINT(readability-magic-numbers)

  // Create compressor and throw error if it fails
  tj_compressor_ = tjInitCompress();
  if (tj_compressor_ == NULL) throw std::runtime_error("Failed to initialize JPEG compressor");

  // Set pixel_format_
  std::map<CompFrameFormat, TJPF> formats = {{CompFrameFormat::kGray, TJPF::TJPF_GRAY}, {CompFrameFormat::kRGB, TJPF::TJPF_RGB}};
  pixel_format_ = formats.at(frame_format);

  // Set comp_flags_
  std::map<CompFrameMethod, int> flags = {{CompFrameMethod::kAccurate, TJFLAG_ACCURATEDCT}, {CompFrameMethod::kFast, TJFLAG_FASTDCT}};
  comp_flags_ = flags.at(comp_method);
}

JpegCompressor::~JpegCompressor() {
  try {
    DestroyCompressor();
  } catch (...) {
  }
}

Compressed JpegCompressor::CompressImage(const unsigned char* uncompressed_image) {
  // Compress image
  unsigned char* compressed = NULL;
  long unsigned int size = size_;
  tjCompress2(tj_compressor_, uncompressed_image, width_, 0, height_, pixel_format_, &compressed, &size, TJSAMP::TJSAMP_444, quality_, comp_flags_);

  // Adjust max size if it is larger
  if (size_ < size) size_ = size;
  return {compressed, size};
}

void JpegCompressor::DestroyCompressor() {
  // Destroy compressor and throw error if it fails
  int success = tjDestroy(tj_compressor_);
  if (success != 0) throw std::runtime_error("Failed to destroy JPEG compressor");
}
