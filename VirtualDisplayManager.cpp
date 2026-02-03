#include "VirtualDisplayManager.hpp"

#ifdef __linux__
    #include "LinuxEvdiManager.hpp"
#elif defined(_WIN32)
    #include "WindowsIddManager.hpp"
#endif