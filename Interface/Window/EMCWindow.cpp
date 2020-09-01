//
// Created by loyusum on 7/6/2020.
//

#include "EMCWindow.hpp"

EMCWindow::EMCWindow(bool fullscreen, int screen_size_x, int screen_size_y) {

    ToolBox::log() << "new EMCWindow Created !" << std::endl;

    if (fullscreen) {
        m_app_size_x = screen_size_x;
        m_app_size_y = screen_size_y;

        m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(),
                                                       "EMCMeeting", sf::Style::Fullscreen);
    } else {
        m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                       "EMCMeeting", sf::Style::Default - sf::Style::Resize);
    }

    ChangeWindowsSize_();
    m_image_renderer = ToolBox::make_unique<ImageRenderer>(sf::Vector2f{0.0f, 0.0f},
                                                           sf::Vector2f{(float) screen_size_x, (float) screen_size_y});}

void EMCWindow::ChangeWindowsSize_() {

}

bool EMCWindow::Update() {

    if (!IsOpen()) {
        return false;
    }

    // for streaming
    auto time_passed_since_start = (float) std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - m_streaming_begin_time).count() *
                                   std::chrono::microseconds::period::num /
                                   std::chrono::microseconds::period::den;
    // update streaming image
    if (!m_image_buffer.empty()) {

        ImageBuffer final_buffer;
        final_buffer.time_since_start = -1;

        // too much delay
        if (m_image_buffer.back().time_since_start - time_passed_since_start > Constant::STREAMING_DELAY) {
            m_streaming_begin_time = std::chrono::system_clock::now() - std::chrono::milliseconds(
                    (int) ((m_image_buffer.back().time_since_start - Constant::STREAMING_DELAY) * 1000.0));

            time_passed_since_start = (float) std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now() - m_streaming_begin_time).count() *
                                      std::chrono::microseconds::period::num /
                                      std::chrono::microseconds::period::den;
        }

        while (!m_image_buffer.empty() && m_image_buffer.begin()->time_since_start <= time_passed_since_start) {
            final_buffer = std::move(*m_image_buffer.begin());
            m_image_buffer.pop_front();
        }

        // we get a image
        if (final_buffer.time_since_start > 0) {
            SetImage(std::move(final_buffer.im));
        }

    }

    // update streaming audio
    if (!m_sound_buffer.empty()) {
        SoundBuffer final_buffer;
        final_buffer.time_since_start = -1;

        // too much delay
        if (m_sound_buffer.back().time_since_start - time_passed_since_start > Constant::STREAMING_DELAY) {
            m_streaming_begin_time = std::chrono::system_clock::now() - std::chrono::milliseconds(
                    (int) ((m_sound_buffer.back().time_since_start - Constant::STREAMING_DELAY) * 1000.0));
        }

        while (!m_sound_buffer.empty() && m_sound_buffer.begin()->time_since_start <= time_passed_since_start) {
            final_buffer = std::move(*m_sound_buffer.begin());
            m_sound_buffer.pop_front();
        }


        // we get a sound buffer
        if (final_buffer.time_since_start > 0) {
            SetSound(std::move(final_buffer.buffer));
        }
    }

    sf::Event app_event{};

    while (m_app->pollEvent(app_event)) {

        switch (app_event.type) {

            case sf::Event::Closed:

                m_app->close();
                break;

            case sf::Event::LostFocus:
                m_is_focusing = false;

                break;

            case sf::Event::GainedFocus:
                m_is_focusing = true;

                break;

            default:
                break;

        }

    }

    m_app->clear();

    m_image_renderer->render(m_app.get());

    m_app->display();

    return true;
}

bool EMCWindow::IsOpen() const {
    return m_app->isOpen();
}

bool EMCWindow::IsOnFocus() {
    return m_app->hasFocus();
}

void EMCWindow::SetTitle(std::string str) {

    m_app->setTitle(str);

}

void EMCWindow::SetImage(sf::Image &&image) {

    //static auto previous_time = std::chrono::system_clock::now();

    /*const auto time_passed = (float) std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - previous_time).count() *
                             std::chrono::microseconds::period::num /
                             std::chrono::microseconds::period::den;

    std::cout << "time_passed : " << time_passed << std::endl;*/

    m_image_renderer->SetImage(image);
    //previous_time = std::chrono::system_clock::now();

}

void EMCWindow::PushBackImageBuffer(EMCWindow::ImageBuffer &&buffer) {

    static bool is_first_image = true;

    if (is_first_image) {

        m_streaming_begin_time = std::chrono::system_clock::now();

        is_first_image = false;
    }

    m_image_buffer.emplace_back(std::move(buffer));
}

void EMCWindow::SetSound(sf::SoundBuffer &&buffer) {

    m_sound_player.setBuffer(buffer);
    m_sound_player.play();
}
