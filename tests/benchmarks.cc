#include <catch2/catch_all.hpp>

#include "camera.hpp"
#include "jpeg_compressor.hpp"
#include "utils.test.hpp"

void TakeImages(Camera& camera) {
  Frame frame = camera.GetFrame();
  delete[] frame.data;
}

void CompressImages(JpegCompressor& compressor, unsigned char* image) {
  Compressed compressed = compressor.CompressImage(image);
  delete[] compressed.frame;
}

TEST_CASE("Benchmark JPEG Compressor") {
  // NOLINTBEGIN (readability-magic-numbers)
  {
    PpmFile file = ReadPpm("../test-images/640x480-test-image.ppm");
    unsigned char* image = new unsigned char[static_cast<unsigned int>(file.width * file.height * 3)];
    for (int i = 0; i < file.width * file.height * 3; i++) image[i] = file.data.at(i);

    JpegCompressor compressor = JpegCompressor(640, 480, 75, CompFrameFormat::kRGB, CompFrameMethod::kFast);
    BENCHMARK("640x480") { return CompressImages(compressor, image); };
  }

  {
    PpmFile file = ReadPpm("../test-images/1280x720-test-image.ppm");
    unsigned char* image = new unsigned char[static_cast<unsigned int>(file.width * file.height * 3)];
    for (int i = 0; i < file.width * file.height * 3; i++) image[i] = file.data.at(i);

    JpegCompressor compressor = JpegCompressor(1280, 720, 75, CompFrameFormat::kRGB, CompFrameMethod::kFast);
    BENCHMARK("1280x720") { return CompressImages(compressor, image); };
  }

  {
    PpmFile file = ReadPpm("../test-images/1920x1080-test-image.ppm");
    unsigned char* image = new unsigned char[static_cast<unsigned int>(file.width * file.height * 3)];
    for (int i = 0; i < file.width * file.height * 3; i++) image[i] = file.data.at(i);

    JpegCompressor compressor = JpegCompressor(1920, 1080, 75, CompFrameFormat::kRGB, CompFrameMethod::kFast);
    BENCHMARK("1920x1080") { return CompressImages(compressor, image); };
  }
  // NOLINTEND (readability-magic-numbers)
}

TEST_CASE("Benchmark Raspi Camera") {
  // NOLINTBEGIN (readability-magic-numbers)
  {
    InputVideoSettings vid_settings = {640, 480, DecompFrameFormat::kRGB};
    CameraSettings cam_settings = {60, 100, 0, 0, 0, 100, 0, false, false, raspicam::RASPICAM_AWB::RASPICAM_AWB_AUTO, raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    Camera camera = Camera(vid_settings, cam_settings);
    BENCHMARK("640x480") { return TakeImages(camera); };
  }

  {
    InputVideoSettings vid_settings = {1280, 720, DecompFrameFormat::kRGB};
    CameraSettings cam_settings = {60, 100, 0, 0, 0, 100, 0, false, false, raspicam::RASPICAM_AWB::RASPICAM_AWB_AUTO, raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    Camera camera = Camera(vid_settings, cam_settings);
    BENCHMARK("1280x720") { return TakeImages(camera); };
  }

  {
    InputVideoSettings vid_settings = {1920, 1080, DecompFrameFormat::kRGB};
    CameraSettings cam_settings = {60, 100, 0, 0, 0, 100, 0, false, false, raspicam::RASPICAM_AWB::RASPICAM_AWB_AUTO, raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    Camera camera = Camera(vid_settings, cam_settings);
    BENCHMARK("1920x1080") { return TakeImages(camera); };
  }
  // NOLINTEND (readability-magic-numbers)
}