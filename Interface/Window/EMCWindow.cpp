//
// Created by loyusum on 7/6/2020.
//

#include "EMCWindow.hpp"

Window::Window(bool fullscreen, int screen_size_x, int screen_size_y) {

    ToolBox::log() << "new Window Created !" << std::endl;

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
}

void Window::ChangeWindowsSize_() {

}

void Window::Update() {

    sf::Event app_event{};

    while (m_app->pollEvent(app_event)) {

        switch (app_event.type) {

            case sf::Event::Closed:

                m_app->close();
                break;

            default:
                break;

        }

    }

    m_app->clear();
    m_app->display();
}

bool Window::IsOpen() const {
    return m_app->isOpen();
}
