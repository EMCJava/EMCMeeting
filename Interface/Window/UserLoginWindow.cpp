//
// Created by loyusum on 7/6/2020.
//

#include "UserLoginWindow.hpp"

UserLoginWindow::UserLoginWindow(int screen_size_x, int screen_size_y) {

    m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                   "EMCMeeting", sf::Style::Default - sf::Style::Resize);
}

bool UserLoginWindow::Update() {
    const auto is_open = m_app->isOpen();

    if (is_open) {

        sf::Event app_event{};

        while (m_app->pollEvent(app_event)) {

            switch (app_event.type) {

                case sf::Event::Closed:

                    m_app->close();
                    break;

                case sf::Event::TextEntered:



                    break;

                default:
                    break;

            }

        }

        m_app->clear();
        m_app->display();

    }

    return is_open;
}
