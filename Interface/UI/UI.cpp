#include "UI.hpp"

UI::UI(sf::Vector2f position)
    : position_(position)
{
    //std::cout << "\033[1;35mUI constructor called !!\033[0m" << std::endl;
}

void UI::setPosition(sf::Vector2f position)
{
    this->position_ = position;
}

sf::Vector2f UI::getPosition() const
{
    return this->position_;
}