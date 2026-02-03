#include "VirtualDisplayManager.hpp"

#ifdef __linux__
    #include "include/EvdiManager/LinuxEvdiManager.hpp"
#elif defined(_WIN32)
    #include "WindowsIddManager.hpp"
#endif

std::unique_ptr<VirtualDisplayManager> VirtualDisplayManager::create() {
#ifdef __linux__
    return std::make_unique<LinuxEvdiManager>();
#elif defined(_WIN32)
    // return std::make_unique<WindowsIddManager>();
    return nullptr; 
#else
    return nullptr;
#endif
}