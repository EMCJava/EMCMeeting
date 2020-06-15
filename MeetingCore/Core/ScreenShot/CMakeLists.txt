add_library(ScreenShotlib
            ScreenShot.cpp
            ScreenShot.hpp)

target_link_libraries(ScreenShotlib X11)
target_link_libraries(ScreenShotlib Xext)