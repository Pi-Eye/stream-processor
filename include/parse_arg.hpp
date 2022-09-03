#ifndef PARSE_ARG_HPP
#define PARSE_ARG_HPP

#include "motion_detector.hpp"
#include "text_overlay.hpp"

/**
 * Settings - Struct containing all the settings for camera
 */
struct Settings {
  InputVideoSettings vid_settings;
  MotionConfig motion_config;
  FontSettings font_settings;
};

/**
 * ParseArgs - Parses out settings from command line and uses defaults if not set
 *
 * argc:    argc from main
 * argv:    argv from main
 * returns: Settings - all settings for camera
 */
Settings ParseArgs(int argc, char** argv);

#endif