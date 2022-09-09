// NOLINTBEGIN(readability-magic-numbers)
#include <catch2/catch_all.hpp>

#include "stream_processor.hpp"
#include "utils.test.hpp"

const int kDevice = 0;  // OpenCL device to run tests on

std::ostream* empty_output = new std::ostream(0);

struct Configs {
  MotionConfig motion;
  std::pair<unsigned int, unsigned int> resolution;
};

// NOLINTBEGIN(readability-*)
std::vector<std::pair<unsigned int, unsigned int>> resolutions = {{320, 240}, {640, 480}, {1280, 720}, {1920, 1080}};
std::vector<unsigned int> gaussian_sizes = {1, 2};
std::vector<unsigned int> scale_denominators = {10, 5};

std::vector<Configs> PermutateConfigs() {
  std::vector<Configs> configs;
  for (int a = 0; a < resolutions.size(); a++) {
    for (int b = 0; b < gaussian_sizes.size(); b++) {
      for (int c = 0; c < scale_denominators.size(); c++) {
        MotionConfig motion = {gaussian_sizes.at(b), scale_denominators.at(c), 10, 2, 10, 0.1, DecompFrameMethod::kFast};
        configs.push_back({motion, resolutions.at(a)});
      }
    }
  }
  return configs;
}
// NOLINTEND(readability-*)

void ProcessFrame(StreamProcessor& processor, JpegFile& jpeg_file) {
  Processed processed = processor.ProcessFrame(jpeg_file.data, jpeg_file.filesize, 0);
  delete[] processed.compressed.frame;
}

TEST_CASE("Benchmark Stream Processor") {
  std::vector<Configs> configs = PermutateConfigs();
  DeviceConfig device_config = {DeviceType::kSpecific, kDevice};
  FontSettings font_settings = {"Test", TextPosition::kTop, true, 9, "../assets/Pixeloid_by_GGBotNet.ttf"};
  for (int i = 0; i < configs.size(); i++) {
    std::string name;
    {  // Create name of benchmark
      name.append(std::to_string(configs.at(i).resolution.first));
      name.append("x");
      name.append(std::to_string(configs.at(i).resolution.second));
      name.append("\n");

      name.append("Gaussian Size: ");
      name.append(std::to_string(configs.at(i).motion.gaussian_size));
      name.append("   Scale: ");
      name.append(std::to_string(configs.at(i).motion.scale_denominator));
      name.append("\n");
    }

    // Get relavent test image
    // NOLINTBEGIN(readability-magic-numbers)
    std::string jpg_name;
    if (configs.at(i).resolution.first == 320) jpg_name = "../test-images/320x240-test-image.jpg";
    if (configs.at(i).resolution.first == 640) jpg_name = "../test-images/640x480-test-image.jpg";
    if (configs.at(i).resolution.first == 1280) jpg_name = "../test-images/1280x720-test-image.jpg";
    if (configs.at(i).resolution.first == 1920) jpg_name = "../test-images/1920x1080-test-image.jpg";

    JpegFile jpeg_frame = ReadJpeg(jpg_name);
    // NOLINTEND(readability-magic-numbers)

    StreamProcessor processor = StreamProcessor(empty_output, configs.at(i).resolution.first, configs.at(i).resolution.second, CompFrameFormat::kRGB, 75, font_settings,
                                                configs.at(i).motion, device_config);

    BENCHMARK(std::string(name)) { return ProcessFrame(processor, jpeg_frame); };
  }
}
// NOLINTEND(readability-magic-numbers)
