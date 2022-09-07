// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <fstream>
#include <iostream>

#include "text_overlay.hpp"
#define ALLOWABLE_ERROR 1

struct PpmFile {
  unsigned int width;
  unsigned int height;
  std::vector<unsigned int> data;
};

PpmFile ReadPpm(const std::string& path) {
  // Create filestream and throw error if it fails
  std::ifstream ifs(path);
  if (!ifs.good()) throw std::runtime_error("Error reading file: " + path);

  // Check header of ppm file and throw error if it is not a ppm file
  std::string header;
  ifs >> header;
  if (header != "P2" && header != "P3") throw std::runtime_error("File: " + path + " was not a valid PPM file");

  // Get height and width of ppm file
  unsigned int height;
  unsigned int width;
  ifs >> width >> height;

  // Determine the number of colors there are
  int color_bytes = 3;
  if (header == "P2") color_bytes = 1;

  // Read in max color value and throw error if it is not as expected
  unsigned int num;
  ifs >> num;
  if (num != 255) throw std::runtime_error("PPM file did not have expected max color value");  // NOLINT(readability-magic-numbers)

  // Read in data and throw error if failure occurs
  int index = 0;
  std::vector<unsigned int> data;
  while (ifs.good()) {
    ifs >> num;
    if (ifs.fail()) throw std::runtime_error("File: " + path + " was not a valid PPM file");

    if (index < height * width * color_bytes) {
      data.push_back(num);
      index++;
    }
  }

  return {width, height, data};
}

bool CompareRGB(PpmFile ppm, const unsigned char* compare) {
  bool match = true;
  for (unsigned int row = 0; row < ppm.height; row++) {
    for (unsigned int col = 0; col < ppm.width; col++) {
      unsigned int index = (row * ppm.width + col) * 3;
      if (abs(static_cast<int>(ppm.data.at(index)) - static_cast<int>(compare[index])) > ALLOWABLE_ERROR) {
        match = false;
        std::cout << "Red pixel did not match at row: " << row << " col: " << col << std::endl;
        std::cout << "Expected: " << ppm.data.at(index) << " but recieved: " << static_cast<int>(compare[index]) << std::endl;
      }
      if (abs(static_cast<int>(ppm.data.at(index + 1)) - static_cast<int>(compare[index + 1])) > ALLOWABLE_ERROR) {
        match = false;
        std::cout << "Green pixel did not match at row: " << row << " col: " << col << std::endl;
        std::cout << "Expected: " << ppm.data.at(index + 1) << " but recieved: " << static_cast<int>(compare[index + 1]) << std::endl;
      }
      if (abs(static_cast<int>(ppm.data.at(index + 2)) - static_cast<int>(compare[index + 2])) > ALLOWABLE_ERROR) {
        match = false;
        std::cout << "Blue pixel did not match at row: " << row << " col: " << col << std::endl;
        std::cout << "Expected: " << ppm.data.at(index + 2) << " but recieved: " << static_cast<int>(compare[index + 2]) << std::endl;
      }
    }
  }
  delete[] compare;
  return match;
}

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