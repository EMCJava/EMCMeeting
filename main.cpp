#include <iostream>

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow main_app(sf::VideoMode(800, 600), "EMCMeeting",
                              sf::Style::Default - sf::Style::Resize);

    while (main_app.isOpen()) {

        sf::Event app_event{};

        while (main_app.pollEvent(app_event)) {

            switch (app_event.type) {

                case sf::Event::Closed:

                    main_app.close();
                    break;

                default:
                    break;

            }

        }

        main_app.clear();
        main_app.display();

    }

    return 0;
}
