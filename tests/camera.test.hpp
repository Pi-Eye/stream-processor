// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <chrono>

#include "camera.hpp"

TEST_CASE("Camera Settings") {
  SECTION("Valid Settings") {
    CameraSettings cam_settings = {30, 100, 0, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    InputVideoSettings settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_NOTHROW(Camera(settings, cam_settings));
  }

  SECTION("Invalid Settings") {
    // FPS
    CameraSettings cam_settings = {0, 100, 0, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    InputVideoSettings settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Brightness
    cam_settings = {30, 101, 0, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Sharpness 1
    cam_settings = {30, 100, -101, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Sharpness 2
    cam_settings = {30, 101, 101, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Contrast 1
    cam_settings = {30, 100, 0, -101, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Contrast 2
    cam_settings = {30, 100, 0, 101, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Saturation 1
    cam_settings = {30, 100, 0, 0, -101, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Saturation 2
    cam_settings = {30, 100, 0, 0, 101, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // ISO 1
    cam_settings = {30, 100, 0, 0, 0, 99, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // ISO 2
    cam_settings = {30, 100, 0, 0, 0, 801, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Exposure Comp 1
    cam_settings = {30, 100, 0, 0, 0, 100, -11, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));

    // Exposure Comp 2
    cam_settings = {30, 100, 0, 0, 0, 100, 11, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
    settings = {640, 480, DecompFrameFormat::kRGB};
    REQUIRE_THROWS(Camera(settings, cam_settings));
  }
}

TEST_CASE("Camera Returns Timestamp Correctly") {
  CameraSettings cam_settings = {30, 100, 0, 0, 0, 100, 0, false, false, RASPICAM_AWB::RASPICAM_AWB_AUTO, RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO};
  InputVideoSettings settings = {640, 480, DecompFrameFormat::kRGB};
  Camera cam = Camera(settings, cam_settings);

  std::chrono::system_clock::time_point time_start = std::chrono::system_clock::now();
  Frame frame = cam.GetFrame();
  REQUIRE(frame.timestamp > std::chrono::duration_cast<std::chrono::seconds>(time_start.time_since_epoch()).count());
}

// NOLINTEND(misc-definitions-in-headers)