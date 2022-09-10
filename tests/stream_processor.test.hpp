// NOLINTBEGIN(misc-definitions-in-headers)
#include <catch2/catch_all.hpp>
#include <fstream>
#include <iostream>

#include "stream_processor.hpp"
#include "utils.test.hpp"

const int kDevice = 0;  // OpenCL device to run tests on

TEST_CASE("Stream Processor") {
  JpegFile jpeg_no_text = ReadJpeg("../test-images/640x480-test-image.jpg");
  JpegFile jpeg_changed = ReadJpeg("../test-images/640x480-test-image-changed.jpg");
  JpegFile jpeg_with_text = ReadJpeg("../test-images/640x480-test-image-with-font.jpg");

  SECTION("Without Text Without Motion") {
    StreamProcessor processor = StreamProcessor();
    processor.SetRequired(640, 480, CompFrameFormat::kRGB, 100);

    for (int i = 0; i < 20; i++) {
      Processed processed = processor.ProcessFrame(jpeg_no_text.data, jpeg_no_text.filesize);

      JpegFile output = {processed.compressed.size, processed.compressed.frame};
      REQUIRE(CompareJpegRGB(jpeg_no_text, output, 640, 480));

      REQUIRE(processed.motion == false);
    }
  }

  SECTION("With Text Without Motion") {
    StreamProcessor processor = StreamProcessor();
    processor.SetRequired(640, 480, CompFrameFormat::kRGB, 100);
    processor.SetFont({"Test", TextPosition::kTop, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"});

    for (int i = 0; i < 20; i++) {
      Processed processed = processor.ProcessFrame(jpeg_no_text.data, jpeg_no_text.filesize);

      JpegFile output = {processed.compressed.size, processed.compressed.frame};
      REQUIRE(CompareJpegRGB(jpeg_with_text, output, 640, 480));

      REQUIRE(processed.motion == false);
    }
  }

  SECTION("With Text With Motion On Every Other Frame") {
    StreamProcessor processor = StreamProcessor();
    processor.SetRequired(640, 480, CompFrameFormat::kRGB, 100);
    processor.SetFont({"Test", TextPosition::kTop, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"});
    processor.SetMotionSettings({0, 1, 10, 1, 10, 0.1, DecompFrameMethod::kAccurate}, {DeviceType::kSpecific, kDevice});
    processor.SetMotionFPSScale(2);

    // Fill up background stabilization
    for (int i = 0; i < 40; i++) {
      Processed processed = processor.ProcessFrame(jpeg_no_text.data, jpeg_no_text.filesize);
      JpegFile output = {processed.compressed.size, processed.compressed.frame};
      REQUIRE(CompareJpegRGB(jpeg_with_text, output, 640, 480));
    }

    // Add in same frames
    for (int i = 0; i < 40; i++) {
      Processed processed = processor.ProcessFrame(jpeg_no_text.data, jpeg_no_text.filesize);
      JpegFile output = {processed.compressed.size, processed.compressed.frame};
      REQUIRE(CompareJpegRGB(jpeg_with_text, output, 640, 480));

      REQUIRE(processed.motion == false);
    }

    // Check that a different frame added causes motion to be detected
    // First frame should not do anything because motion runs at 1/2 fps
    Processed processed0 = processor.ProcessFrame(jpeg_changed.data, jpeg_changed.filesize);
    REQUIRE(processed0.motion == false);
    // Second should have motion
    Processed processed1 = processor.ProcessFrame(jpeg_changed.data, jpeg_changed.filesize);
    REQUIRE(processed1.motion == true);
  }
}
// NOLINTEND(misc-definitions-in-headers)