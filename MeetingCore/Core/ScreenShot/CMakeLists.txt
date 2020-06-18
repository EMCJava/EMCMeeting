add_library(ScreenShotlib
            ScreenShot.cpp
            ScreenShot.hpp)

if(UNIX)
target_link_libraries(ScreenShotlib X11)
target_link_libraries(ScreenShotlib Xext)
endif()