//
// Created by samsa on 6/17/2020.
//

#include <SFML/Graphics.hpp>

#include <deque>
#include <chrono>
#include "../MeetingCore/Core/AudioCapturer/AudioCapturer.hpp"

#define time_use(x) {\
    auto start_time = std::chrono::system_clock::now();\
    x;\
    std::cout << "Time used " << ((double) std::chrono::duration_cast<std::chrono::microseconds>(\
    std::chrono::system_clock::now() - start_time).count() *\
    std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << std::endl;\
}

int main() {

    std::deque<Socket::Message> audio_message_buffer;
    AudioCapturer audioCapturer(&audio_message_buffer, 1.0f, 0.0f);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Uniy Test");

    audioCapturer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    audioCapturer.Stop();

    sf::SoundBuffer soundBuffer;
    MessagePackage::ReadSoundBuffer(audio_message_buffer[0], soundBuffer);

    sf::Sound sound(soundBuffer);
    sound.play();

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
        window.display();
    }

    return 0;
}