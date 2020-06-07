#include "InputField.hpp"

InputField::InputField(std::string id, std::string font_name, std::string init_text, float loca_x, float loca_y, int width, int height, int text_size)
    : Button(id, font_name, init_text, loca_x, loca_y, width, height, text_size, 2, 5)
    , text_str_(init_text)
{
    std::cout << "\033[1;35mInputField constructor called !!\033[0m" << std::endl;
}

InputField::~InputField()
{
}

bool InputField::isConcentrated() const
{
    return is_on_concentration_;
}

void InputField::push_back(char character)
{
    std::string str_tem = text_str_;

    switch (character) {

    case static_cast<char>(8):
        //delete key
        if (!text_str_.empty())
            text_str_.pop_back();
        break;

    default:
        // if not inside text period
        if (character < ' ' || character > '~')
            break;

        text_str_.push_back(character);
    }

    if (text_str_.compare(str_tem)) {
        if (text_str_.empty()) {
            Button::setText("|");
            this->alignText();
            Button::setText("");

        } else {
            Button::setText(text_str_);
            this->alignText();
        }
    }
}

std::string InputField::getText() const
{
    return text_str_;
}

void InputField::setText(const std::string str)
{
    text_str_ = str;

    if (text_str_.empty()) {
        Button::setText("|");
        this->alignText();
        Button::setText("");
    } else {
        Button::setText(str);
        this->alignText();
    }
}

void InputField::setText(const std::wstring str)
{
    text_str_ = std::string(str.begin(), str.end());

    if (text_str_.empty()) {
        Button::setText("|");
        this->alignText();
        Button::setText("");
    } else {
        Button::setText(str);
        this->alignText();
    }
}

std::string InputField::getId()
{
    return Button::getId();
}

void InputField::render(sf::RenderWindow* win, float timer)
{
    if (is_on_concentration_) {
        if (sin((timer - last_time_since_selected_) / (CLOCKS_PER_SEC * 0.1)) > 0.3f) {
            Button::setText(text_str_ + "|");
        } else {
            Button::setText(text_str_);
        }
    }

    Button::render(win);
}

void InputField::update(sf::RenderWindow* win, float timer)
{
    Button::update(win);

    if (this->isPressed()) {
        last_time_since_selected_ = timer;
        is_on_concentration_ = true;

        this->button_sprite_.setOutlineThickness(3);
    } else if (is_on_concentration_) {
        if (!(is_on_concentration_ = !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
                    || this->_isMouseStayOnButton(win))) {
            Button::setText(text_str_);

            this->button_sprite_.setOutlineThickness(2);
        }
    }
}

void InputField::display(sf::RenderWindow* win, float timer)
{
    update(win, timer);
    render(win, timer);
}