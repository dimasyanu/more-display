add_library(library)

target_sources(library
    PRIVATE
        VirtualDisplayManager.cpp
    PUBLIC
        include/Constants/edid_data.hpp
        include/VirtualDisplayManager/VirtualDisplayManager.hpp
        include/EvdiManager/LinuxEvdiManager.hpp
)

target_include_directories(library PUBLIC include)

# Link against EVDI and Threading library
target_link_libraries(more-display
    ${EVDI_LIB}
    pthread
)

# add_library(more-display::library)