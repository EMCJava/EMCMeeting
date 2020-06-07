#ifndef INPUT_FIELD_H_
#define INPUT_FIELD_H_

#include <ctime>

#include <SFML/Graphics.hpp>

#include "../Button/Button.hpp"
#include "../UI.hpp"

class InputField : protected Button {
private:
    float last_time_since_selected_ = 0;

    bool is_on_concentration_ = false;

    std::string text_str_;
public:
    InputField(std::string id, std::string font_name, std::string init_text, float loca_x, float loca_y, int width, int height, int text_size = 12);
    ~InputField();

    bool isConcentrated() const;
    void push_back(char character);

    std::string getText() const;

    void setText(const std::string str);
    void setText(const std::wstring str);

    std::string getId();

    /*
     * Render the input field and text
    */
    void render(sf::RenderWindow* win, float timer = 0);

    /*
     *Update the input field
    */
    void update(sf::RenderWindow* win, float timer = 0);

    /*
     *Update and Render the input field
    */
    void display(sf::RenderWindow* win, float timer = 0);
};

#endif