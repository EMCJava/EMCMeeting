//
// Created by loyusum on 9/6/2020.
//

#include "UserAskWhetherHosterWindow.hpp"

const std::string UserAskWhetherHosterWindow::sm_font_path = "resource/ShareTechMono.ttf";

UserAskWhetherHosterWindow::UserAskWhetherHosterWindow(int screen_size_x, int screen_size_y) {

    m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                   "User Type", sf::Style::Default - sf::Style::Resize);

    if (!TextRenderer::addFont(sm_font_path, "default")) {

        ToolBox::err() << "Can't load font : " << sm_font_path << std::endl;
    }

    m_buttons.emplace_back("client", "default", "Client", 50, 20, 125, 40, 25);
    m_buttons.emplace_back("hoster", "default", "Hoster", 225, 20, 125, 40, 25);

    m_buttons.emplace_back("cancel", "default", "Cancel", 137.5, 100, 125, 40, 25);
}

bool UserAskWhetherHosterWindow::Update() {

    const auto is_open = m_app->isOpen();

    if (is_open) {

        sf::Event app_event{};

        while (m_app->pollEvent(app_event)) {

            switch (app_event.type) {

                case sf::Event::Closed:

                    Close();
                    return false;

                default:
                    break;

            }

        }

        m_app->clear(sf::Color(0, 152, 152));

        for (auto &button : m_buttons) {

            button.display(m_app.get());

            if (button.isPressed()) {

                const auto id = button.getId();

                if (id.compare("client") == 0) {

                    m_userType = UserType::Client;

                } else if (id.compare("hoster") == 0) {

                    m_userType = UserType::Hoster;

                } else {

                    m_userType = UserType::None;

                }

                Close();
                return false;
            }
        }

        m_app->display();

    }

    return is_open;

}

void UserAskWhetherHosterWindow::Close() {
    WindowBase::Close();
}

UserAskWhetherHosterWindow::UserType UserAskWhetherHosterWindow::GetType() {
    return m_userType;
}
