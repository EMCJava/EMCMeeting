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
    m_image_renderer = ToolBox::make_unique<ImageRenderer>(sf::Vector2f{0.0f, 0.0f},
                                                           sf::Vector2f{(float) screen_size_x, (float) screen_size_y});
}

void Window::ChangeWindowsSize_() {

}

bool Window::Update() {

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

bool Window::IsOpen() const {
    return m_app->isOpen();
}

bool Window::IsOnFocus() {
    return m_app->hasFocus();
}

void Window::SetTitle(std::string str) {

    m_app->setTitle(str);

}

void Window::SetImage(sf::Image image) {

    m_image_renderer->SetImage(image);

}
