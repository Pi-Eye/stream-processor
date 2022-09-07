
#include "parse_arg.hpp"

#include <map>

#include "camera.hpp"
#include "jpeg_decompressor.hpp"
#include "motion_detector.hpp"
#include "text_overlay.hpp"

Settings ParseArgs(int argc, char** argv) {
  // Parse arguments from cmd line
  std::map<std::string, std::string> arguments = {{"--width", "640"},  // Default arguments
                                                  {"--height", "480"},
                                                  {"--format", "G"},
                                                  {"--decomp", "fast"},
                                                  {"--fps", "24"},
                                                  {"--brightness", "100"},
                                                  {"--sharpness", "0"},
                                                  {"--contrast", "0"},
                                                  {"--saturation", "0"},
                                                  {"--iso", "100"},
                                                  {"--excomp", "0"},
                                                  {"--hflip", "n"},
                                                  {"--vflip", "n"},
                                                  {"--awb", "0"},
                                                  {"--exposure", "0"},
                                                  {"--camname", "Camera"},
                                                  {"--position", "B"},
                                                  {"--showdate", "y"},
                                                  {"--fontsize", "8"},
                                                  {"--font", "Pixeloid_by_GGBotNet.ttf"},
                                                  {"--gaussian", "1"},
                                                  {"--scale", "10"},
                                                  {"--bgstabil", "30"},
                                                  {"--mvtstabil", "2"},
                                                  {"--pixthresh", "3"},
                                                  {"--framethresh", "0.1"}};

  // Pull out any flags that were set
  bool arg_identifier = false;
  for (int i = 1; i < argc; i++) {
    if (argv[i - 1][0] == '-' && argv[i - 1][1] == '-') {
      if (arguments.contains(std::string(argv[i - 1]))) {
        arguments.at(std::string(argv[i - 1])) = std::string(argv[i]);
      }
    }
  }

  // Determine frame formts and methods
  std::map<std::string, std::pair<CompFrameFormat, DecompFrameFormat>> formats = {{"RGB", {CompFrameFormat::kRGB, DecompFrameFormat::kRGB}},
                                                                                  {"G", {CompFrameFormat::kGray, DecompFrameFormat::kGray}}};
  std::map<std::string, std::pair<CompFrameMethod, DecompFrameMethod>> methods = {{"fast", {CompFrameMethod::kFast, DecompFrameMethod::kFast}},
                                                                                  {"accurate", {CompFrameMethod::kAccurate, DecompFrameMethod::kAccurate}}};
  std::map<std::string, bool> true_false = {{"y", true}, {"n", false}};

  // Camera Settings
  std::map<std::string, raspicam::RASPICAM_AWB> white_balance = {{"off", raspicam::RASPICAM_AWB::RASPICAM_AWB_OFF},
                                                                 {"auto", raspicam::RASPICAM_AWB::RASPICAM_AWB_AUTO},
                                                                 {"sunlight", raspicam::RASPICAM_AWB::RASPICAM_AWB_SUNLIGHT},
                                                                 {"cloudy", raspicam::RASPICAM_AWB::RASPICAM_AWB_CLOUDY},
                                                                 {"shade", raspicam::RASPICAM_AWB::RASPICAM_AWB_SHADE},
                                                                 {"tungsten", raspicam::RASPICAM_AWB::RASPICAM_AWB_TUNGSTEN},
                                                                 {"fluorescent", raspicam::RASPICAM_AWB::RASPICAM_AWB_FLUORESCENT},
                                                                 {"incandescent", raspicam::RASPICAM_AWB::RASPICAM_AWB_INCANDESCENT},
                                                                 {"flash", raspicam::RASPICAM_AWB::RASPICAM_AWB_FLASH},
                                                                 {"horizon", raspicam::RASPICAM_AWB::RASPICAM_AWB_HORIZON}};
  std::map<std::string, raspicam::RASPICAM_AWB> exposure_modes = {{"off", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_OFF},
                                                                  {"auto", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_AUTO},
                                                                  {"night", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_NIGHT},
                                                                  {"nightpreview", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_NIGHTPREVIEW},
                                                                  {"backlight", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_BACKLIGHT},
                                                                  {"spotlight", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_SPOTLIGHT},
                                                                  {"sports", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_SPORTS},
                                                                  {"snow", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_SNOW},
                                                                  {"beach", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_BEACH},
                                                                  {"verylong", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_VERYLONG},
                                                                  {"fixedfps", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_FIXEDFPS},
                                                                  {"antishake", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_ANTISHAKE},
                                                                  {"fireworks", raspicam::RASPICAM_EXPOSURE::RASPICAM_EXPOSURE_FIREWORKS}};
  unsigned int fps = std::stoi(arguments.at("--fps"));
  unsigned int brightness = std::stoi(arguments.at("--brightness"));
  int sharpness = std::stoi(arguments.at("--sharpness"));
  int contrast = std::stoi(arguments.at("--contrast"));
  unsigned int iso = std::stoi(arguments.at("--iso"));
  int exposure_comp = std::stoi(arguments.at("--excomp"));
  bool borizontal_flip = true_false.at(arguments.at("--hflip"));
  bool vertical_flip = true_false.at(arguments.at("--vflip"));
  raspicam::RASPICAM_AWB awb = white_balance.at(arguments.at("--awb"));
  raspicam::RASPICAM_EXPOSURE exposure_mode = exposure_modes.at(arguments.at("--exposure"));

  // Input Video Settings
  DecompFrameFormat decomp_format = formats.at(arguments.at("--format")).second;
  DecompFrameMethod decomp_method = methods.at(arguments.at("--decomp")).second;
  unsigned int width = std::stoi(arguments.at("--width"));
  unsigned int height = std::stoi(arguments.at("--height"));
  InputVideoSettings vid_settings = {width, height, decomp_format};

  // Motion Config
  unsigned int gaussian = std::stoi(arguments.at("--gaussian"));
  unsigned int scale = std::stoi(arguments.at("--scale"));
  unsigned int bg_stabil = std::stoi(arguments.at("--bgstabil"));
  unsigned int mvt_stabil = std::stoi(arguments.at("--mvtstabil"));
  unsigned int min_pix_diff = std::stoi(arguments.at("--pixthresh"));
  float min_changed_pix = std::stof(arguments.at("--framethresh"));
  MotionConfig motion_config = {gaussian, scale, bg_stabil, mvt_stabil, min_pix_diff, min_changed_pix, decomp_method};

  // Font Settings
  std::map<std::string, TextPosition> positions = {{"T", TextPosition::kTop}, {"B", TextPosition::kBottom}};
  std::string cam_name = arguments.at("--camname");
  TextPosition position = positions.at(arguments.at("--position"));
  bool show_date = true_false.at(arguments.at("--showdate"));
  unsigned int font_size = std::stoi(arguments.at("--fontsize"));
  std::string font = arguments.at("--font");
  FontSettings font_settings = {cam_name, position, show_date, font_size, font};

  return {vid_settings, motion_config, font_settings};
}