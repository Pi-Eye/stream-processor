// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <fstream>
#include <iostream>

#include "jpeg_compressor.hpp"
#include "jpeg_decompressor.hpp"
#include "utils.test.hpp"

TEST_CASE("Jpeg Compressor") {
  PpmFile image = ReadPpm("../test-images/bottom-cst.ppm");
  unsigned char* blank = new unsigned char[image.width * image.height * 3];
  for (int i = 0; i < image.width * image.height * 3; i++) blank[i] = static_cast<unsigned char>(image.data.at(i));

  JpegCompressor compressor = JpegCompressor(image.width, image.height, 75, CompFrameFormat ::kRGB, CompFrameMethod::kAccurate);
  Compressed compressed = compressor.CompressImage(blank);

  JpegDecompressor decompressor = JpegDecompressor(image.width, image.height, DecompFrameFormat::kRGB, DecompFrameMethod::kAccurate);
  unsigned char* b = decompressor.DecompressImage(compressed.frame, compressed.size);
}

// NOLINTEND(misc-definitions-in-headers)