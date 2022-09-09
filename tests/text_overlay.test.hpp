// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>

#include "text_overlay.hpp"
#include "utils.test.hpp"

TEST_CASE("Text Overlay") {
  SECTION("Top") {
    PpmFile expected = ReadPpm("../test-images/top-cst.ppm");
    unsigned char* blank = new unsigned char[expected.width * expected.height * 3];
    for (int i = 0; i < expected.width * expected.height * 3; i++) blank[i] = static_cast<unsigned char>(255);

    InputVideoSettings vid_settings = {200, 16, DecompFrameFormat::kRGB};
    FontSettings settings = {"Test", TextPosition::kTop, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"};
    TextOverlay text_overlay = TextOverlay(vid_settings, settings);

    text_overlay.OverlayOnFrame(blank, 0);
    REQUIRE(CompareRGB(expected, blank));
  }

  SECTION("Bottom") {
    PpmFile expected = ReadPpm("../test-images/bottom-cst.ppm");
    unsigned char* blank = new unsigned char[expected.width * expected.height * 3];
    for (int i = 0; i < expected.width * expected.height * 3; i++) blank[i] = static_cast<unsigned char>(255);

    InputVideoSettings vid_settings = {200, 16, DecompFrameFormat::kRGB};
    FontSettings settings = {"Test", TextPosition::kBottom, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"};
    TextOverlay text_overlay = TextOverlay(vid_settings, settings);

    text_overlay.OverlayOnFrame(blank, 0);
    REQUIRE(CompareRGB(expected, blank));
  }

  SECTION("Overflow") {
    PpmFile expected = ReadPpm("../test-images/bottom-overflow-cst.ppm");
    unsigned char* blank = new unsigned char[expected.width * expected.height * 3];
    for (int i = 0; i < expected.width * expected.height * 3; i++) blank[i] = static_cast<unsigned char>(255);

    InputVideoSettings vid_settings = {200, 16, DecompFrameFormat::kRGB};
    FontSettings settings = {"A REALLY LONG NAME", TextPosition::kBottom, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"};
    TextOverlay text_overlay = TextOverlay(vid_settings, settings);

    text_overlay.OverlayOnFrame(blank, 0);
    REQUIRE(CompareRGB(expected, blank));
  }
}

// NOLINTEND(misc-definitions-in-headers)