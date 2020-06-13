//
// Created by loyusum on 9/6/2020.
//

#include "UserClientAskHosterIPAndPortWindow.hpp"

const std::string UserClientAskHosterIPAndPortWindow::sm_font_path = "resource/ShareTechMono.ttf";

UserClientAskHosterIPAndPortWindow::UserClientAskHosterIPAndPortWindow(int screen_size_x, int screen_size_y) {

    m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                   "Login", sf::Style::Default - sf::Style::Resize);

    if (!TextRenderer::addFont(sm_font_path, "default")) {

        ToolBox::err() << "Can't load font : " << sm_font_path << std::endl;
    }

    // set the input layout
    m_input_fields.emplace_back("server_ip", "default", "127.0.0.1",
                                175, 10, 215, 40, 15);

    m_input_fields.emplace_back("server_port", "default", "20546",
                                175, 70, 215, 40, 30);

    // set default user name and password
    ip = m_input_fields[0].getText();
    port = m_input_fields[1].getText();

    m_buttons = ToolBox::make_unique<Button>("ok", "default", "Connect", 100, 140, 200, 40, 25);

    m_texts = ToolBox::make_unique<TextRenderer>();
    m_texts->addText("server_ip_text", "Server ip :", "default", 20, 10, 10);
    m_texts->addText("server_port_text", "Server port :", "default", 20, 10, 70);
}

bool UserClientAskHosterIPAndPortWindow::Update() {

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

        m_texts->render(m_app.get());
        m_buttons->display(m_app.get());

        if (m_buttons->isPressed()) {

            m_is_cancel = false;
            Close();
            return false;
        }

        for (auto &field : m_input_fields) {
            field.display(m_app.get(), clock());
        }

        m_app->display();

    }

    return is_open;

}

void UserClientAskHosterIPAndPortWindow::text_input(const char character) {

    if (m_input_fields[0].isConcentrated()) {
        m_input_fields[0].push_back(character);

        ip = m_input_fields[0].getText();

        // ip input ui change color
        m_input_fields[0].setTextColor(Socket::IsValidIP(ip) ? sf::Color(0, 204, 0) : sf::Color::Red);
    }

    if (m_input_fields[1].isConcentrated()) {
        m_input_fields[1].push_back(character);

        port = m_input_fields[1].getText();
    }

}

void UserClientAskHosterIPAndPortWindow::Close() {
    ip = m_input_fields[0].getText();
    port = m_input_fields[1].getText();

    WindowBase::Close();
}

bool UserClientAskHosterIPAndPortWindow::IsCancel() {
    return m_is_cancel;
}
