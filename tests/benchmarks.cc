
#include <catch2/catch_all.hpp>

#include "camera.hpp"

void TakeImages(Camera& camera) {
  unsigned char* image = camera.GetFrame();
  delete image;
}

TEST_CASE("Benchmark Raspi Camera") {
  SECTION("Time To Take Frame") {
    // NOLINTBEGIN (readability-magic-numbers)
    InputVideoSettings vid_settings = {640, 480, DecompFrameFormat::kRGB};
    CameraSettings cam_settings = {60, 100, 0, 0, 0, 100, 0, false, false, raspicam::RASPICAM_AWB::RASPICAM_AWB_AUTO, raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    Camera camera = Camera(vid_settings, cam_settings);
    BENCHMARK("640x480") { return TakeImages(camera); };

    vid_settings = {1260, 720, DecompFrameFormat::kRGB};
    camera = Camera(vid_settings, cam_settings);
    BENCHMARK("1260x720") { return TakeImages(camera); };

    vid_settings = {1920, 1080, DecompFrameFormat::kRGB};
    camera = Camera(vid_settings, cam_settings);
    BENCHMARK("1920x1080") { return TakeImages(camera); };
    // NOLINTEND (readability-magic-numbers)
  }
}