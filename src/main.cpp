#include <iostream>
#include "fitusb.hpp"
int main(int argc, char** argv) {
    if(argc < 4) return -2;
    auto device = argv[1];
    auto file = argv[2];
    auto bs = atoi(argv[3]);
    return fitusb::flash(device, file, bs);
}