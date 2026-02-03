#include "VirtualDisplayManager.hpp"
#include <iostream>

int main() {
    auto manager = VirtualDisplayManager::create();

    if (!manager->initialize()){
        std::cerr << "Failed to find EVDI device. Is modprobe evdi run?" << std::endl;
        return 1;
    }

    manager->setFrameCallback([](const ScreenFrame& frame) {
        std::cout << "Captured frame! Buffer size: " << frame.size << " bytes" << std::endl;
        // This is where you will eventually add FFmpeg encoding
    });

    std::cout << "Starting virtual display 1080p..." << std::endl;
    manager->createDisplay("MobileStream", 1920, 1080);
    manager->run(); 

    return 0;
}
