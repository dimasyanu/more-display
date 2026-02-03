add_executable(moredisplay ${SOURCES})
target_sources(moredisplay
    PRIVATE
        main.cpp
)

target_link_libraries(moredisplay
    PRIVATE
        moredisplay_lib::library
)