//
// Created by loyusum on 15/6/2020.
//

#include "ScreenShot.hpp"

ScreenShot::ScreenShot() {

#ifdef __linux__

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

#elif defined(__WIN32__) || defined(_WIN32)

    m_desktop_hwnd = GetDesktopWindow();

    RECT DesktopParams;
    GetWindowRect(m_desktop_hwnd, &DesktopParams);

    m_width = DesktopParams.right - DesktopParams.left;
    m_height = DesktopParams.bottom - DesktopParams.top;

    // get rgb_buffer size
    m_dev_c = GetDC(m_desktop_hwnd);
    m_capture_c = CreateCompatibleDC(m_dev_c);
    m_capture_bitmap = CreateCompatibleBitmap(m_dev_c, m_width, m_height);
    SelectObject(m_capture_c, m_capture_bitmap);

    m_bmp_info.bmiHeader.biSize = sizeof(m_bmp_info.bmiHeader);
    GetDIBits(m_capture_c, m_capture_bitmap, 0, m_height, NULL, &m_bmp_info, DIB_RGB_COLORS);

    rgb_buffer.resize(m_bmp_info.bmiHeader.biSizeImage);

    m_success_attach = true;

#endif
}

bool ScreenShot::GetScreenShot(sf::Image &image) {

    if (!m_success_attach)
        return false;

#ifdef __linux__

    XShmGetImage(m_display, m_window_root, m_ximg, 0, 0, 0x00ffffff);

    const auto image_prt_size = m_width * m_height * 4;

    for (int i = 0; i < image_prt_size; i += 4) {

        std::swap(m_ximg->data[i], m_ximg->data[i + 2]);
        m_ximg->data[i + 3] = (char) 255; // make it not transparent

    }

    image.create(m_width, m_height, reinterpret_cast<const sf::Uint8 *>(m_ximg->data));

    return true;

#elif defined(__WIN32__) || defined(_WIN32)

    BitBlt(m_capture_c, 0, 0, m_width, m_height, m_dev_c, 0, 0, SRCCOPY | CAPTUREBLT);

    //Create BITMAPINFO variable, set size
    BITMAPINFO MyBMInfo = {0};
    MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

    //Get the BITMAPINFO structure from the bitmap
    if (0 == GetDIBits(m_capture_c, m_capture_bitmap, 0, 0, nullptr, &MyBMInfo, DIB_RGB_COLORS)) {
        // error handling
        return false;
    }

    //Create the bitmap pixel array each element is [b,g,r]
    //BYTE *lpPixels = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
    if (MyBMInfo.bmiHeader.biSizeImage != rgb_buffer.size()) {

        rgb_buffer.resize(MyBMInfo.bmiHeader.biSizeImage);
    }

    //Setting up the structure of the buffer to be received
    MyBMInfo.bmiHeader.biCompression = BI_RGB;  // No-compression

    //Now get the actual data from the picture
    if (0 ==
        GetDIBits(m_capture_c, m_capture_bitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID) rgb_buffer.data(), &MyBMInfo,
                  DIB_RGB_COLORS)) {
        // error handling
        return false;
    }

    //Now create an array of SFML pixels we want to fill
    auto *lpPixelWithAlpha = new sf::Uint8[MyBMInfo.bmiHeader.biSizeImage +
                                           (MyBMInfo.bmiHeader.biSizeImage / 3) / 3]; //Add room for alpha

    //Loop through each pixel, with steps of four RGBA!
    for (int x = 0; x < MyBMInfo.bmiHeader.biSizeImage; x += 4) {
        lpPixelWithAlpha[x] = rgb_buffer[x + 2];    //lpPixels = r
        lpPixelWithAlpha[x + 1] = rgb_buffer[x + 1];    //lpPixels = g
        lpPixelWithAlpha[x + 2] = rgb_buffer[x];    //lpPixels = b
        lpPixelWithAlpha[x + 3] = 255;        //Nada alpha (just to adjust if you like)
    }

    //Load picture, now with correct pixels and alpha channel
    image.create(MyBMInfo.bmiHeader.biWidth,
                 MyBMInfo.bmiHeader.biHeight, lpPixelWithAlpha);
    image.flipVertically();

    //Remove the pixels with alphachannel
    delete[] lpPixelWithAlpha;

    //Notify ok!
    return true;

#endif
}

ScreenShot::~ScreenShot() {

    ReleaseDC(::GetDesktopWindow(), m_dev_c);
    DeleteObject(m_capture_bitmap);
    DeleteDC(m_capture_c);

}
