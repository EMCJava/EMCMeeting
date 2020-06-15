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
                                                           sf::Vector2f{(float) screen_size_x, (float) screen_size_y});
}

void EMCWindow::ChangeWindowsSize_() {

}

bool EMCWindow::Update() {

    if (!IsOpen()) {
        return false;
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

void EMCWindow::SetImage(sf::Image image) {

    static auto previous_time = std::chrono::system_clock::now();

    const auto time_passed = (float) std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - previous_time).count() *
                             std::chrono::microseconds::period::num /
                             std::chrono::microseconds::period::den;

    std::cout << "time_passed : " << time_passed << std::endl;

    m_image_renderer->SetImage(image);
    previous_time = std::chrono::system_clock::now();

}
