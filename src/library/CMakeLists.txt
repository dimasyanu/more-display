add_library(library)

target_sources(library
    PRIVATE
        VirtualDisplayManager.cpp
    PUBLIC
        include/Constants/edid_data.hpp
        include/VirtualDisplayManager/VirtualDisplayManager.hpp
)


# Platform specific logic
if (APPLE)

elseif(WIN32)

else()
    # Linux
    list(APPEND SOURCES include/LinuxEvdiManager.hpp)
endif()

target_include_directories(library PUBLIC include)

# Link against EVDI and Threading library
target_link_libraries(more-display
    ${EVDI_LIB}
    pthread
)

add_library(moredisplay_lib::library ALIAS library)