#include <iostream>

#include "camera.hpp"
#include "jpeg_compressor.hpp"
#include "motion_detector.hpp"
#include "text_overlay.hpp"

// Argument list: -w [width] -h [height] -f [format: <RGB> <G>]
//                -n [camera name] -p [text position: <TL> <TR> <BL> <BR>] -d [show date: <y> <n>]
//                -g [gaussian size] -s [scale denominator]
//                -b [background stabilization length] -m [movement stabilization length]
//                -p [minimum pixel difference] -c [minimum changed pixels]
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