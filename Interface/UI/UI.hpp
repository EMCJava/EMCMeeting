#ifndef UI_H_
#define UI_H_

#include <SFML/Graphics.hpp>

#include <iostream>

class UI {
protected:
    sf::Vector2f position_;

public:
    UI(sf::Vector2f position);

    virtual void setPosition(sf::Vector2f position);
    virtual sf::Vector2f getPosition() const;
};

#endif