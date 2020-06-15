//
// Created by loyusum on 15/6/2020.
//

#include "ScreenShot.hpp"

ScreenShot::ScreenShot() {

    m_display = XOpenDisplay(nullptr);
    m_window_root = DefaultRootWindow(m_display);  // Macro to return the root window! It's a simple uint32
    XGetWindowAttributes(m_display, m_window_root, &m_window_attributes);

    m_width = sf::VideoMode::getDesktopMode().width;
    m_height = sf::VideoMode::getDesktopMode().height;

    m_screen = m_window_attributes.screen;
    m_ximg = XShmCreateImage(m_display, DefaultVisualOfScreen(m_screen), DefaultDepthOfScreen(m_screen), ZPixmap, NULL,
                             &m_shminfo, m_width, m_height);

    m_shminfo.shmid = shmget(IPC_PRIVATE, m_ximg->bytes_per_line * m_ximg->height, IPC_CREAT | 0777);
    m_shminfo.shmaddr = m_ximg->data = (char *) shmat(m_shminfo.shmid, 0, 0);
    m_shminfo.readOnly = False;

    if (m_shminfo.shmid < 0)
        ToolBox::err() << "Fatal shminfo error!" << std::endl;

    Status s1 = XShmAttach(m_display, &m_shminfo);
    ToolBox::log() << "XShmAttach() " << ((m_success_attach = s1) ? "success!" : "failure!") << std::endl;
}

bool ScreenShot::GetScreenShot(sf::Image &image) {

    if (!m_success_attach)
        return false;

    XShmGetImage(m_display, m_window_root, m_ximg, 0, 0, 0x00ffffff);

    const auto image_prt_size = m_width * m_height * 4;

    for (int i = 0; i < image_prt_size; i += 4) {

        std::swap(m_ximg->data[i], m_ximg->data[i + 2]);
        m_ximg->data[i + 3] = (char) 255; // make it not transparent

    }

    image.create(m_width, m_height, reinterpret_cast<const sf::Uint8 *>(m_ximg->data));

    return true;
}
