#include "text_overlay.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <iostream>

#include "motion_detector.hpp"

#define PX_SCALE 64         // FreeType takes in px in 1/64ths
#define POINTS_PER_INCH 72  // How many pt per inch for font

TextOverlay::TextOverlay(InputVideoSettings input_vid, FontSettings font_settings) : vid_settings_(input_vid), font_settings_(font_settings) {
  FT_Error error;
  FT_Library library;

  // Load library
  error = FT_Init_FreeType(&library);
  if (error) throw std::runtime_error("Failed to initizalize library with error code: " + std::to_string(error));

  // Load font file
  error = FT_New_Face(library, font_settings.font_filename.c_str(), 0, &font_face_);
  if (error) throw std::runtime_error("Failed to create font face with error code: " + std::to_string(error));

  // Set font size
  // NOLINTNEXT(bugprone-misplaced-widening-cast)
  error = FT_Set_Char_Size(font_face_, 0, static_cast<FT_F26Dot6>(font_settings.font_size * PX_SCALE), POINTS_PER_INCH, POINTS_PER_INCH);
  if (error) throw std::runtime_error("Failed to set character size with error code: " + std::to_string(error));

  // Set where to start writing
  start_x_ = 1;
  if (font_settings_.text_position == TextPosition::kTop) start_y_ = font_settings.font_size;
  if (font_settings_.text_position == TextPosition::kBottom) start_y_ = vid_settings_.height - 2;

  // Find number of colors
  if (vid_settings_.frame_format == DecompFrameFormat::kGray) colors_ = 1;
  if (vid_settings_.frame_format == DecompFrameFormat::kRGB) colors_ = 3;

  FT_GlyphSlot slot = font_face_->glyph;
  FT_UInt glyph_index;

  FT_Load_Char(font_face_, 'T', FT_LOAD_RENDER);
  FT_Bitmap* bitmap = &slot->bitmap;
}

void TextOverlay::OverlayOnFrame(unsigned char* frame, long long timestamp) const {
  std::string text = GenerateText(timestamp);

  unsigned int pen_x = start_x_;
  unsigned int pen_y = start_y_;

  FT_GlyphSlot glyph = font_face_->glyph;

  // Figure out where the text ends
  int pen_x_end = 0;
  for (unsigned int i = 0; i < text.size(); i++) {
    FT_Error error = FT_Load_Char(font_face_, text.at(i), FT_LOAD_RENDER);
    if (error) continue;
    pen_x_end += glyph->advance.x >> 6;
  }
  if (pen_x_end == 0) return;

  for (unsigned int y = pen_y - font_settings_.font_size; y <= pen_y + 1 && y < vid_settings_.height && 0 <= y; y++) {
    for (unsigned int x = 0; x < vid_settings_.width && x < pen_x_end + 1; x++) {
      int buff_loc = (y * vid_settings_.width + x) * colors_;
      for (unsigned int c = 0; c < colors_; c++) {
        frame[buff_loc + c] *= 0.3;
      }
    }
  }

  for (unsigned int i = 0; i < text.size(); i++) {
    // Load character
    FT_Error error = FT_Load_Char(font_face_, text.at(i), FT_LOAD_RENDER);
    if (error) continue;
    FT_Bitmap* bitmap = &glyph->bitmap;
    for (unsigned int y = 0, draw_y = pen_y - glyph->bitmap_top; y < bitmap->rows && draw_y < vid_settings_.height && 0 <= draw_y; y++, draw_y++) {
      for (unsigned int x = 0, draw_x = pen_x + glyph->bitmap_left; x < bitmap->width && draw_x < vid_settings_.width && 0 <= draw_x; x++, draw_x++) {
        int buff_loc = (draw_y * vid_settings_.width + draw_x) * colors_;
        for (unsigned int c = 0; c < colors_; c++) {
          if (bitmap->buffer[x + bitmap->width * y] != 0) frame[buff_loc + c] = 255;
        }
      }
    }

    pen_x += glyph->advance.x >> 6;
  }
}

std::string TextOverlay::GenerateText(long long timestamp) const {
  auto epoch = std::chrono::time_point<std::chrono::system_clock>();
  auto oldNow = epoch + std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<int>(timestamp));
  auto t_c = std::chrono::system_clock::to_time_t(oldNow);

  std::stringstream time_string;
  time_string << std::put_time(std::localtime(&t_c), "%F %T %Z");
  return font_settings_.cam_name + " " + time_string.str();
}
