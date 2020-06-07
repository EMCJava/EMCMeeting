//
// Created by loyusum on 7/6/2020.
//

#include "UserLoginWindow.hpp"

const std::string UserLoginWindow::sm_font_path = "resource/ShareTechMono.ttf";

UserLoginWindow::UserLoginWindow(int screen_size_x, int screen_size_y) {

    m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                   "Login", sf::Style::Default - sf::Style::Resize);

    if (!TextRenderer::addFont(sm_font_path, "default")) {

        ToolBox::err() << "Can't load font : " << sm_font_path << std::endl;
    }

    // set the input layout
    m_input_fields.emplace_back("user_name_id", "default", "user name",
                                50, 10, 300, 40, 30);

    m_input_fields.emplace_back("password_id", "default", "password",
                                50, 70, 300, 40, 30);

    // set default user name and password
    user_name = m_input_fields[0].getText();
    password = m_input_fields[1].getText();

    m_buttons = ToolBox::make_unique<Button>("ok", "default", "Login", 100, 140, 200, 40, 25);
}

bool UserLoginWindow::Update() {
    const auto is_open = m_app->isOpen();

    if (is_open) {

        sf::Event app_event{};

        while (m_app->pollEvent(app_event)) {

            switch (app_event.type) {

                case sf::Event::Closed:

                    Close();
                    return false;

                case sf::Event::TextEntered:

                    text_input(app_event.text.unicode);

                    break;

                default:
                    break;

            }

        }

        m_app->clear(sf::Color(0, 152, 152));

        m_buttons->display(m_app.get());

        if (m_buttons->isPressed()) {

            Close();
            return false;
        }

        for (auto &field : m_input_fields) {
            field.display(m_app.get(), clock());
            sf::Text a;
            a.setCharacterSize(1);
        }

        m_app->display();

    }

    return is_open;
}

void UserLoginWindow::text_input(const char character) {

    if (m_input_fields[0].isConcentrated()) {
        m_input_fields[0].push_back(character);

        user_name = m_input_fields[0].getText();
    }

    if (m_input_fields[1].isConcentrated()) {
        m_input_fields[1].push_back(character);

        password = m_input_fields[1].getText();
    }
}

void UserLoginWindow::Close() {
    user_name = m_input_fields[0].getText();
    password = m_input_fields[1].getText();

    m_app->close();
}
