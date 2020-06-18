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

#elif defined(__WIN32__) || defined(_WIN32)

#include <Windows.h>

#else

#error only for linux x11 system and window

#endif

class ScreenShot {

private:

#ifdef __linux__

    Display *m_display;
    Window m_window_root;
    XWindowAttributes m_window_attributes;

    Screen *m_screen;
    XShmSegmentInfo m_shminfo;
    XImage *m_ximg;

    unsigned int m_width, m_height;

#elif defined(__WIN32__) || defined(_WIN32)

    DWORD m_width, m_height;
    HWND m_desktop_hwnd;
    BITMAPINFO m_bmp_info = {0};
    HDC m_dev_c, m_capture_c;
    HBITMAP m_capture_bitmap;

    std::vector<char> rgb_buffer;

#endif

    bool m_success_attach = false;

public:

    ScreenShot();

    ~ScreenShot();

    bool GetScreenShot(sf::Image &image);

};


#endif //EMCMEETING_SCREENSHOT_HPP
