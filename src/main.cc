#include <iostream>

#include "camera.hpp"
#include "motion_detector.hpp"
#include "parse_arg.hpp"
#include "text_overlay.hpp"

// Argument list:
// --width [width] --height [height] --format [format: <RGB> <G>] --decomp [<fast> <accurate>] --fps [fps] --brightness [0, 100] --sharpness [-100, 100] 
// --contrast [-100, 100] --saturation [-100, 100] --iso [100, 800] --excomp [-10, 10] --hflip [flip horizontally <y> <n>] --vflip [flip vertically <y> <n>] 
// --awb [<off> <auto> <sunlight> <cloudy> <shade> <tungsten> <fluorescent> <incandescent> <flash> <horizon>]
// --exposure [<off> <auto> <night> <nightpreview> <backlight> <spotlight> <sports> <snow> <beach> <verylong> <fixedfps> <antishake> <fireworks>]
// --camname [camera name] --position [text position: <TL> <TR> <BL> <BR>] --showdate [show date: <y> <n>] --fontsize [font size (px)] --font [font file]
// --gaussian [gaussian size] --scale [scale denominator] --bgstabil [background stabilization length] --mvtstabil [movement stabilization length]
// --pixthresh [minimum pixel difference] --framethresh [minimum changed pixels in a frame]
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