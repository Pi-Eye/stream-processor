#include <iostream>

#include "camera.hpp"
#include "jpeg_compressor.hpp"
#include "motion_detector.hpp"
#include "parse_arg.hpp"
#include "text_overlay.hpp"

// Argument list: --width [width] --height [height] --format [format: <RGB> <G>] --decomp [<fast> <accurate>] --comp [<fast> <accurate>]
//                --camname [camera name] --position [text position: <TL> <TR> <BL> <BR>] --showdate [show date: <y> <n>] --fontsize [font size (px)] --font [font file]
//                --gaussian [gaussian size] --scale [scale denominator] --bgstabil [background stabilization length] --mvtstabil [movement stabilization length]
//                --pixthresh [minimum pixel difference] --framethresh [minimum changed pixels in a frame]
int main(int argc, char** argv) {
  try {
    std::cout << "Hello, World" << std::endl;

    // Catch and print all execptions
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return -1;
  } catch (const std::string& ex) {
    std::cerr << ex << std::endl;
    return -1;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
    return -1;
  }
}