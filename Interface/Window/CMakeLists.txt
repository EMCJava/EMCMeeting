add_library(EMCWindowlib
            EMCWindow.cpp
            EMCWindow.hpp)

add_library(WindowBaselib INTERFACE)
target_sources(WindowBaselib INTERFACE WindowBase.hpp)

if (UNIX)
    target_include_directories(EMCWindowlib PUBLIC ${SFML_INCLUDE_DIRS})
endif ()