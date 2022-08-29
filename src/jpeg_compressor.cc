
#include "jpeg_compressor.hpp"

#include <turbojpeg.h>

JpegCompressor::JpegCompressor(unsigned int width, unsigned int height, CompFrameFormat frame_format, CompFrameMethod comp_method) {}

JpegCompressor::~JpegCompressor() {}

Compressed JpegCompressor::CompressImage(const unsigned char* uncompressed_image) {}

void JpegCompressor::DestroyCompressor() {}
