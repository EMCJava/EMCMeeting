#ifndef BUTTON_H_
#define BUTTON_H_

#include "../TextRenderer/TextRenderer.hpp"
#include "../UI.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

class Button : public UI {
protected:
    bool is_pressing_ = false, is_holding_ = false, have_pressed_ = false;

    std::string id_;

    sf::ConvexShape button_sprite_;
    TextRenderer text_;

    int button_width, button_height;

    bool _isMouseStayOnButton(sf::RenderWindow* win);
    void _initButtonSprite(int edge_point_count, float edge_radius);

    void (*trigger_)(Button* button, void* ex_parameter) = nullptr;

public:
    Button(std::string id, std::string font_name, std::wstring text, float loca_x, float loca_y, int width, int height, int text_size = 12, int edge_point_count = 2, float edge_radius = 10);
    Button(std::string id, std::string font_name, std::string text, float loca_x, float loca_y, int width, int height, int text_size = 12, int edge_point_count = 2, float edge_radius = 10);
    ~Button();

    std::string getId() const;

    void setTrigger(void (*trigger)(Button* button, void* ex_parameter) = nullptr);
    void setPosition(sf::Vector2f position);
    void setText(const std::string str);
    void setText(const std::wstring str);

    std::wstring getText() const;

    void alignText();
    void resetState();

    /*
     *reset button's state after calling this function
    */
    bool isPressed();

    bool isPressing(sf::RenderWindow* win) const;

    /*
     *update the state of the button
    */
    void update(sf::RenderWindow* win);

    /*
     *Render the button and text
    */
    void render(sf::RenderWindow* win);

    /*
     *update and render the button
    */
    void display(sf::RenderWindow* win);
};

#endif