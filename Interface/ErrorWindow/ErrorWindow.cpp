//
// Created by loyusum on 9/6/2020.
//

#include "ErrorWindow.hpp"

#include <utility>

const std::string ErrorWindow::sm_font_path = "resource/ShareTechMono.ttf";


ErrorWindow::ErrorWindow(int screen_size_x, int screen_size_y, std::string error_mes) {

    m_app = ToolBox::make_unique<sf::RenderWindow>(sf::VideoMode(screen_size_x, screen_size_y),
                                                   "Error", sf::Style::Default - sf::Style::Resize);

    if (!TextRenderer::addFont(sm_font_path, "default")) {

        ToolBox::err() << "Can't load font : " << sm_font_path << std::endl;
    }

    m_texts = ToolBox::make_unique<TextRenderer>();
    m_texts->addText("error", "Error", "default", 20, screen_size_x / 2, 10);
    m_texts->addText("mes", std::move(error_mes), "default", 20, screen_size_x / 2, 70);

    m_texts->alignText("error");
    m_texts->alignText("mes");

}

bool ErrorWindow::Update() {

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

        m_texts->render(m_app.get());

        m_app->display();

    }

    return is_open;

}

void ErrorWindow::Close() {
    WindowBase::Close();
}
