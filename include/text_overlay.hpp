#ifndef TEXT_OVERLAY_HPP
#define TEXT_OVERLAY_HPP

#include "motion_detector.hpp"

/**
 * TextPosition - Where to place text on frame
 */
enum class TextPosition { kTopLeft, kTopRight, kBottomLeft, kBottomRight };

/**
 * FontSettings - Settings for the text and font
 *
 * font_size:     Size of the font to use
 * font_filename: Filename of font to use
 * text_position: Position to place text on frame
 */
struct FontSettings {
  unsigned int font_size;
  std::string font_filename;
  TextPosition text_position;
};

/**
 * TextOverlay - Overlays some text over a frame
 */
class TextOverlay {
 public:
  /**
   * TextOverlay() - Constructor for TextOverlay
   *
   * input_vid:    Settings of the frames to overlay text onto
   */
  TextOverlay(InputVideoSettings input_vid, FontSettings font);

  /**
   * OverlayOnFrame() - Overlays text over the given frame
   *
   * frame:     Frame to overlay text onto
   * text:      Text to overlay
   */
  void OverlayOnFrame(unsigned char* frame, const std::string& text) const;

 private:
  FontSettings font;             // Font settings
  InputVideoSettings input_vid;  // Format of the frames to overlay text onto
};

#endif