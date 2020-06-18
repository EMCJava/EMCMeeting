//
// Created by samsa on 6/17/2020.
//

#include <string>
#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>

#include <chrono>
#include "../MeetingCore/Core/ScreenShot/ScreenShot.hpp"

#define time_use(x) {\
    auto start_time = std::chrono::system_clock::now();\
    x;\
    std::cout << "Time used " << ((double) std::chrono::duration_cast<std::chrono::microseconds>(\
    std::chrono::system_clock::now() - start_time).count() *\
    std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << std::endl;\
}

/*class ScreenShot {

    DWORD Width, Height;
    HWND DesktopHwnd;
    BITMAPINFO bmpInfo = {0};
    HDC DevC;

    std::vector<char> rgb_buffer;

public:
    ScreenShot() {

        DesktopHwnd = GetDesktopWindow();

        RECT DesktopParams;
        GetWindowRect(DesktopHwnd, &DesktopParams);

        Width = DesktopParams.right - DesktopParams.left;
        Height = DesktopParams.bottom - DesktopParams.top;

        // get rgb_buffer size
        DevC = GetDC(DesktopHwnd);
        HDC CaptureDC = CreateCompatibleDC(DevC);
        HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, Width, Height);
        bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
        GetDIBits(CaptureDC, CaptureBitmap, 0, Height, NULL, &bmpInfo, DIB_RGB_COLORS);

        rgb_buffer.resize(bmpInfo.bmiHeader.biSizeImage);
        ReleaseDC(nullptr, DevC);
        ReleaseDC(nullptr, CaptureDC);
    }

    bool shot(sf::Image &im) {

        DevC = GetDC(DesktopHwnd);

        HDC CaptureDC = CreateCompatibleDC(DevC);
        HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, Width, Height);

        SelectObject(CaptureDC, CaptureBitmap);
        BitBlt(CaptureDC, 0, 0, Width, Height, DevC, 0, 0, SRCCOPY | CAPTUREBLT);

        //Create a DC to get hBitmap information
        HDC hDC = GetDC(::GetDesktopWindow());

        //Create BITMAPINFO variable, set size
        BITMAPINFO MyBMInfo = {0};
        MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

        //Get the BITMAPINFO structure from the bitmap
        if (0 == GetDIBits(CaptureDC, CaptureBitmap, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS)) {
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
            GetDIBits(CaptureDC, CaptureBitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID) rgb_buffer.data(), &MyBMInfo,
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
        im.create(MyBMInfo.bmiHeader.biWidth,
                  MyBMInfo.bmiHeader.biHeight, lpPixelWithAlpha);

        //Remove the pixels with alphachannel
        delete[] lpPixelWithAlpha;

        //Release the DC
        ReleaseDC(::GetDesktopWindow(), hDC);
        ReleaseDC(nullptr, DevC);


        //Notify ok!
        return true;
    }
};
*/

int main() {
    ScreenShot screenShot;

    sf::Image im;
    time_use(screenShot.GetScreenShot(im))
    im.flipVertically();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Uniy Test");

    sf::Texture tx;
    tx.loadFromImage(im);

    sf::Sprite sp(tx);

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered
        // since the last iteration of the loop
        // `event` is a reserved keyword in C++,
        // hence why use `e` instead
        sf::Event e;

        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        screenShot.GetScreenShot(im);
        im.flipVertically();
        tx.loadFromImage(im);

        window.draw(sp);

        window.display();
    }

    return 0;
}