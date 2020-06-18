add_library(AskHosterIPAndPortWindowlib
        AskHosterIPAndPortWindow.cpp
        AskHosterIPAndPortWindow.hpp)

if (UNIX)
    target_include_directories(AskHosterIPAndPortWindowlib PUBLIC ${SFML_INCLUDE_DIRS})
endif ()