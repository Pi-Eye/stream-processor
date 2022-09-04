// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "parse_arg.hpp"

std::pair<int, char**> StringToArgv(std::string string) {
  std::vector<std::string> args;

  std::string temp;
  for (int i = 0; i < string.size(); i++) {
    if (string.at(i) == ' ') {
      args.push_back(temp);
      temp = "";
    } else {
      temp.push_back(string.at(i));
    }
  }
  args.push_back(temp);

  char** argv = new char*[args.size()];
  for (int i = 0; i < args.size(); i++) {
    argv[i] = new char[args.at(i).size() + 1];
    for (int j = 0; j < args.at(i).size(); j++) {
      argv[i][j] = args.at(i).at(j);
    }
    argv[i][args.at(i).size()] = '\0';
  }
  return {args.size(), argv};
}

TEST_CASE("Parse Arguments") {
  SECTION("With No Arguments") {
    std::string string = "/path/to/some/exec";
    std::pair<int, char**> args = StringToArgv(string);

    REQUIRE_NOTHROW(ParseArgs(args.first, args.second));
  }

  SECTION("With Invalid Arguments") {
    std::string string = "/path/to/some/exec --not a --real argument";
    std::pair<int, char**> args = StringToArgv(string);

    REQUIRE_NOTHROW(ParseArgs(args.first, args.second));
  }

  SECTION("With Valid Arguments") {
    std::string string =
        "/path/to/some/exec --width 1920 --height 1080 --format RGB --decomp fast --fps 30 --sharpness 10 --contrast 10 --saturation 10 --iso 400 --excomp 1 --hflip y "
        "--vflip y --awb sunlight --exposure night --camname camera --position BR --showdate n --fontsize 19 --font /path/to/font --gaussian 4 --scale 23 --bgstabil 123 "
        "--mvtstabil 12 --pixthresh 24 --framethresh 0.9";
    std::pair<int, char**> args = StringToArgv(string);

    REQUIRE_NOTHROW(ParseArgs(args.first, args.second));
    Settings settings = ParseArgs(args.first, args.second);

    REQUIRE(settings.cam_settings.fps == 30);
    REQUIRE(settings.cam_settings.sharpness == 30);
    REQUIRE(settings.cam_settings.contrast == 30);
    REQUIRE(settings.cam_settings.saturation == 30);
    REQUIRE(settings.cam_settings.exposure_comp == 30);
    REQUIRE(settings.cam_settings.horizontal_flip == 30);
    REQUIRE(settings.cam_settings.vertical_flip == 30);
    REQUIRE(settings.cam_settings.awb == raspicam::RASPICAM_AWB::RASPICAM_AWB_SUNLIGHT);
    REQUIRE(settings.cam_settings.exposure_mode == raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_NIGHT);

    REQUIRE(settings.vid_settings.width == 1920);
    REQUIRE(settings.vid_settings.height == 1080);
    REQUIRE(settings.vid_settings.frame_format == DecompFrameFormat::kRGB);

    REQUIRE(settings.motion_config.bg_stabil_length == 123);
    REQUIRE(settings.motion_config.decomp_method == DecompFrameMethod::kFast);
    REQUIRE(settings.motion_config.gaussian_size == 4);
    REQUIRE(settings.motion_config.scale_denominator == 23);
    REQUIRE(abs(settings.motion_config.min_changed_pixels - 0.9) < 0.01);
    REQUIRE(settings.motion_config.min_pixel_diff == 24);

    REQUIRE(settings.font_settings.cam_name == "camera");
    REQUIRE(settings.font_settings.font_filename == "/path/to/font");
    REQUIRE(settings.font_settings.font_size == 19);
    REQUIRE(settings.font_settings.show_date == false);
    REQUIRE(settings.font_settings.text_position == TextPosition::kBottomRight);
  }
}

// NOLINTEND(misc-definitions-in-headers)