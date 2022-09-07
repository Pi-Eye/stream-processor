#ifndef TEXT_OVERLAY_HPP
#define TEXT_OVERLAY_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

#include "motion_detector.hpp"

/**
 * TextPosition - Where to place text on frame
 */
enum class TextPosition { kTop, kBottom };

/**
 * FontSettings - Settings for the text and font
 *
 * font_size:     Size of the font to use
 * font_filename: Filename of font to use
 * text_position: Position to place text on frame
 */
struct FontSettings {
  std::string cam_name;
  TextPosition text_position;
  bool show_date;
  unsigned int font_size;
  std::string font_filename;
};

/**
 * TextOverlay - Overlays some text over a frame
 */
class TextOverlay {
 public:
  /**
   * TextOverlay() - Constructor for TextOverlay
   *
   * input_vid:       Settings of the frames to overlay text onto
   * font_settings:   Settings for the font to overlay
   */
  TextOverlay(InputVideoSettings input_vid, FontSettings font_settings);

  /**
   * OverlayOnFrame() - Overlays text over the given frame
   *
   * frame:     Frame to overlay text onto
   * timestamp: Timestamp of time to overlay
   */
  void OverlayOnFrame(unsigned char* frame, long long timestamp) const;

 private:
  /**
   * GenerateText() - Generates the text to display on the frame
   *
   * timestamp:   Timestamp of frame
   * returns:     std::string - text to display on frame
   */
  std::string GenerateText(long long timestamp) const;

  unsigned int start_x_;  // Where to start drawing text
  unsigned int start_y_;  // Where to start drawing text
  unsigned int colors_;   // Number of colors in image

  FT_Face font_face_;           // Font face being used
  FontSettings font_settings_;  // Font settings

  InputVideoSettings vid_settings_;  // Incoming frame information
};

#endif