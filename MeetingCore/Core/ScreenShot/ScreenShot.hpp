//
// Created by loyusum on 15/6/2020.
//

#ifndef EMCMEETING_SCREENSHOT_HPP
#define EMCMEETING_SCREENSHOT_HPP

#include <SFML/Graphics.hpp>
#include "../../../ToolBox/ToolBox.hpp"

#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>

#else

#error only for linux x11 system

#endif

class ScreenShot {

private:
    Display *m_display;
    Window m_window_root;
    XWindowAttributes m_window_attributes;

    Screen *m_screen;
    XShmSegmentInfo m_shminfo;
    XImage *m_ximg;

    unsigned int m_width, m_height;

    bool m_success_attach = false;

public:

    ScreenShot();

    bool GetScreenShot(sf::Image& image);

};


#endif //EMCMEETING_SCREENSHOT_HPP
