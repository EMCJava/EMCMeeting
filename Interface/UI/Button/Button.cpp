#include "Button.hpp"

bool Button::_isMouseStayOnButton(sf::RenderWindow* win)
{
    auto mouse_pos = sf::Mouse::getPosition(*win);

    return mouse_pos.x > position_.x - 2
        && mouse_pos.y > position_.y - 2
        && mouse_pos.x < button_width + position_.x + 2
        && mouse_pos.y < button_height + position_.y + 2;
}

void Button::_initButtonSprite(int edge_point_count, float edge_radius)
{
    int centre_x, centre_y;

    std::vector<sf::Vector2f> vertex;

    vertex.push_back({ edge_radius, 0.0f });
    vertex.push_back({ button_width - edge_radius, 0.0f });

    centre_x = button_width - edge_radius;
    centre_y = edge_radius;

    //bottom right
    for (int i = 1; i <= edge_point_count; ++i) {
        float angle = (float)(i * 3.141592653f) / (2.0f * (edge_point_count + 1));

        float vertex_x = centre_x + edge_radius * static_cast<float>(sin(angle));
        float vertex_y = centre_y - edge_radius * static_cast<float>(cos(angle));

        vertex.push_back({ vertex_x, vertex_y });
    }

    vertex.push_back({ static_cast<float>(button_width), edge_radius });
    vertex.push_back({ static_cast<float>(button_width), button_height - edge_radius });

    centre_x = button_width - edge_radius;
    centre_y = button_height - edge_radius;

    //to 0right
    for (int i = edge_point_count; i > 0; --i) {
        float angle = (float)(i * 3.141592653f) / (2.0f * (edge_point_count + 1));

        float vertex_x = centre_x + edge_radius * static_cast<float>(sin(angle));
        float vertex_y = centre_y + edge_radius * static_cast<float>(cos(angle));

        vertex.push_back({ vertex_x, vertex_y });
    }

    vertex.push_back({ button_width - edge_radius, static_cast<float>(button_height) });
    vertex.push_back({ edge_radius, static_cast<float>(button_height) });

    centre_x = edge_radius;
    centre_y = button_height - edge_radius;

    //top left
    for (int i = 1; i <= edge_point_count; ++i) {
        float angle = (float)(i * 3.141592653f) / (2.0f * (edge_point_count + 1));

        float vertex_x = centre_x - edge_radius * static_cast<float>(sin(angle));
        float vertex_y = centre_y + edge_radius * static_cast<float>(cos(angle));

        vertex.push_back({ vertex_x, vertex_y });
    }

    vertex.push_back({ 0, button_height - edge_radius });
    vertex.push_back({ 0, edge_radius });

    centre_x = edge_radius;
    centre_y = edge_radius;

    //bottom left
    for (int i = edge_point_count; i > 0; --i) {
        float angle = (float)(i * 3.141592653f) / (2.0f * (edge_point_count + 1));

        float vertex_x = centre_x - edge_radius * static_cast<float>(sin(angle));
        float vertex_y = centre_y - edge_radius * static_cast<float>(cos(angle));

        vertex.push_back({ vertex_x, vertex_y });
    }

    this->button_sprite_.setPointCount(vertex.size());

    for (int i = 0; i < vertex.size(); i++) {
        this->button_sprite_.setPoint(i, vertex[i]);
    }

    //this->button_sprite_.setPoint(1, { static_cast<float>(button_width), 0.0f });
    //this->button_sprite_.setPoint(2, { static_cast<float>(button_width), static_cast<float>(button_height) });
    //this->button_sprite_.setPoint(3, { 0.0f, static_cast<float>(button_height) });

    this->button_sprite_.setOutlineThickness(2);
    this->button_sprite_.setOutlineColor(sf::Color::Black);

    this->button_sprite_.setFillColor(sf::Color::White);

    this->button_sprite_.setPosition(position_.x, position_.y);
}

Button::Button(std::string id, std::string font_name, std::string text, float loca_x, float loca_y, int width, int height, int text_size, int edge_point_count, float edge_radius)
    : Button(id, font_name, std::wstring(text.begin(), text.end()), loca_x, loca_y, width, height, text_size, edge_point_count, edge_radius)
{
}

Button::Button(std::string id, std::string font_name, std::wstring text, float loca_x, float loca_y, int width, int height, int text_size, int edge_point_count, float edge_radius)
    : id_(id)
    , UI({ loca_x, loca_y })
    , text_(loca_x, loca_y)
    , button_width(width)
    , button_height(height)
{
    //std::cout << "\033[1;35mButton constructor called !!\033[0m" << std::endl;

    this->_initButtonSprite(edge_point_count, edge_radius);
    text_.addText("button_text", text, font_name, text_size, button_width / 2.0f, button_height / 2.0f);
    //text_.addText("button_text", text, font_name, 12, 0, 0);
    text_.alignText("button_text");
}

Button::~Button()
{
}

std::string Button::getId() const
{
    return id_;
}

void Button::setTrigger(void (*trigger)(Button* button, void* ex_parameter))
{
    this->trigger_ = trigger;
}

void Button::setText(const std::wstring str)
{
    text_.setText("button_text", str);
}

void Button::setText(const std::string str)
{
    text_.setText("button_text", str);
}

std::wstring Button::getText() const
{
    return this->text_.getText("button_text");
}

void Button::alignText()
{
    this->text_.alignText("button_text");
}

void Button::resetState()
{
    have_pressed_ = false;
    is_pressing_ = false;
    is_holding_ = false;
}

void Button::setPosition(sf::Vector2f position)
{
    this->position_ = position;
    this->button_sprite_.setPosition(position_.x, position_.y);
    text_.setPosition(position);
}

bool Button::isPressed()
{
    if (have_pressed_) {
        return !(have_pressed_ = false);
    } else {
        return have_pressed_;
    }
}

bool Button::isPressing(sf::RenderWindow* win) const
{
    return is_pressing_;
}

void Button::render(sf::RenderWindow* win)
{
    win->draw(this->button_sprite_);
    text_.render(win);
}

void Button::update(sf::RenderWindow* win)
{
    if ((is_pressing_ = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))) {

        is_pressing_ = is_holding_ = _isMouseStayOnButton(win);

    } else {

        if (is_holding_) {
            have_pressed_ = true;

            if (trigger_ != nullptr)
                trigger_(this, nullptr);
        }

        is_holding_ = false;
    }

    if (is_holding_) {
        this->button_sprite_.setFillColor(sf::Color(200, 200, 200));
    } else {
        this->button_sprite_.setFillColor(sf::Color::White);
    }
}

void Button::display(sf::RenderWindow* win)
{
    this->update(win);
    this->render(win);
}

void Button::setTextColor(sf::Color color) {

    text_.setTextColor(color);

}
