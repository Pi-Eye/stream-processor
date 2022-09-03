// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <chrono>

#include "camera.hpp"

TEST_CASE("Camera Settings") {
  SECTION("Valid Settings") {}

  SECTION("Invalid Settings") {}
}

TEST_CASE("Camera Returns Timestamp Correctly") {
  CameraSettings cam_settings = {30, 100, 0, 0, 0, 100, 0, false, false};
  InputVideoSettings settings = {640, 480, DecompFrameFormat::kRGB};
  Camera cam = Camera(settings, cam_settings);

  std::chrono::system_clock::time_point time_start = std::chrono::system_clock::now();
  Frame frame = cam.GetFrame();
  REQUIRE(frame.timestamp > std::chrono::duration_cast<std::chrono::seconds>(time_start.time_since_epoch()).count());
}

// NOLINTEND(misc-definitions-in-headers)