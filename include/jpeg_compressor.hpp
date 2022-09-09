#ifndef JPEG_COMPRESSOR_HPP
#define JPEG_COMPRESSOR_HPP

#include <turbojpeg.h>

/**
 * CompFrameFormat - Selector for what format uncompressed images are in
 *
 * kRGB:  < Row: < Pixel: <Red Byte, Green Byte, Blue Byte> ...> ...>
 * kGray: < Row: < Pixel: <Grayscale Byte> ...> ...>
 */
enum class CompFrameFormat { kRGB, kGray };

/**
 * CompFrameMethod - Selector for method to use for compressing images
 *
 * kFast:      faster but less accurate method
 * kAccurate:  slower but more accurate method
 */
enum class CompFrameMethod { kFast, kAccurate };

/**
 * Compressed - Struct to hold frame and its size together
 *
 * frame:     data for JPEG
 * size:      size of buffer
 */
struct Compressed {
  unsigned char* frame;
  unsigned long size;
};

/**
 * JpegCompressor - Compressess JPEG images into selected format
 */
class JpegCompressor {
 public:
  /**
   * JpegCompressor() - Constructor for JpegCompressor
   *
   * width:         Width of image to compress
   * height:        Height of image to compress
   * quality:       JPEG quality [0-100]
   * frame_format:  Format to decompress images into
   * decomp_method: Method to use to decompress images
   */
  JpegCompressor(int width, int height, int quality, CompFrameFormat frame_format, CompFrameMethod comp_method);

  /**
   * ~JpegCompressor - Deconstructor for JpegCompressor
   */
  ~JpegCompressor();

  /**
   * CompressImage() - Compresses a JPEG image
   *
   * uncompressed_image:  Uncompressed image to compress in the format specified at construction
   * returns:             Compressed - contains compressed image and its size
   */
  Compressed CompressImage(const unsigned char* uncompressed_image);

 private:
  /**
   * DestroyCompressor() - Destroys the Compressor for JpegCompressor
   */
  void DestroyCompressor();

  int width_;                   // Width of image
  int height_;                  // Height of image
  int quality_;                 // Quality of image
  long unsigned int size_ = 0;  // Size of jpeg

  tjhandle tj_compressor_;  // TurboJPEG image decompressor handle
  TJPF pixel_format_;       // TurboJPEG pixel format of incoming uncompressed images
  int comp_flags_;          // TurboJPEG flags for compressor
};

#endif