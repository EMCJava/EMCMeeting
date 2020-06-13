#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_

#include "../UI.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <map>
#include <tuple>
#include <vector>

class TextRenderer : public UI {
private:
    /* std::string for the name
     * sf::Font for the font itself
    */
    static std::map<std::string, sf::Font> fonts_;
    static sf::Text render_text;
    
    /* std::string for the id
     * {
     *  *[0] for text
     *  *[1] for font name
     *  *[2] for font size
     *  *[3] for location x
     *  *[4] for location y
     *  *[5] for alignment x
     *  *[6] for alignment y
     * }
    */
    std::map<std::string, std::tuple<std::wstring, std::string, int, int, int, int, int>> texts_;

    sf::Color m_text_color = sf::Color::Black;

    static bool _font_exists(const std::string font_name);
    void packSFText(std::string id, sf::Text& text);

public:
    TextRenderer(sf::Vector2f position);
    TextRenderer(float loca_x = 0, float loca_y = 0);
    ~TextRenderer();

    static bool addFont(std::string font_path, std::string font_name);
    static const sf::Font* const getFont(std::string font_name);

    bool addText(std::string id, std::wstring text, std::string font_name, int font_size, int loca_x, int loca_y);
    bool addText(std::string id, std::string text, std::string font_name, int font_size, int loca_x, int loca_y);
    bool removeText(std::string id);

    bool setText(std::string id, std::string text);
    bool setText(std::string id, std::wstring text);
    std::wstring getText(std::string id) const;

    void render(sf::RenderWindow* win);

    void alignText(std::string id);

    void setTextColor(sf::Color color);
};

#endif