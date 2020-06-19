//
// Created by samsa on 6/17/2020.
//

#include <string>
#include <iostream>
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