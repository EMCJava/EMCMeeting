#include "TextRenderer.hpp"

std::map<std::string, sf::Font> TextRenderer::fonts_;
sf::Text TextRenderer::render_text;

TextRenderer::TextRenderer(sf::Vector2f position)
        : UI(position) {
    //std::cout << "\033[1;35mTextRenderer constructor called !!\033[0m" << std::endl;
}

TextRenderer::TextRenderer(float loca_x, float loca_y)
        : TextRenderer::TextRenderer(sf::Vector2f{loca_x, loca_y}) {
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::_font_exists(const std::string font_name) {
    return fonts_.find(font_name) != fonts_.end();
}

void TextRenderer::packSFText(std::string id, sf::Text &text) {
    std::wstring text_str;
    std::string font_name;
    int text_size, text_loca_x, text_loca_y, alignment_x, alignment_y;

    std::tie(text_str, font_name, text_size, text_loca_x, text_loca_y, alignment_x, alignment_y) = texts_.at(id);

    text.setString(text_str);
    text.setCharacterSize(text_size);
    text.setPosition(static_cast<float>(text_loca_x + alignment_x), static_cast<float>(text_loca_y + alignment_y));
}

bool TextRenderer::addFont(std::string font_path, std::string font_name) {
    if (_font_exists(font_name)) {
        return true;
    }

    if (!fonts_[font_name].loadFromFile(font_path)) {
        fonts_.erase(font_name);

        return false;
    }

    return true;
}

const sf::Font *const TextRenderer::getFont(std::string font_name) {
    if (!_font_exists(font_name)) {
        return nullptr;
    }

    return &(fonts_[font_name]);
}

bool
TextRenderer::addText(std::string id, std::string text, std::string font_name, int font_size, int loca_x, int loca_y) {
    return this->addText(id, std::wstring(text.begin(), text.end()), font_name, font_size, loca_x, loca_y);
}

bool
TextRenderer::addText(std::string id, std::wstring text, std::string font_name, int font_size, int loca_x, int loca_y) {
    if (!this->_font_exists(font_name) || texts_.find(id) != texts_.end()) {
        return false;
    }

    texts_[id] = {text, font_name, font_size, loca_x, loca_y, 0, 0};

    return true;
}

bool TextRenderer::removeText(std::string id) {
    if (texts_.find(id) == texts_.end()) {
        return false;
    }

    texts_.erase(id);

    return true;
}

bool TextRenderer::setText(std::string id, std::string text) {
    return this->setText(id, std::wstring(text.begin(), text.end()));
}

bool TextRenderer::setText(std::string id, std::wstring text) {
    if (texts_.find(id) == texts_.end()) {
        return false;
    }

    std::get<0>(texts_.at(id)) = text;

    return true;
}

std::wstring TextRenderer::getText(std::string id) const {
    if (texts_.find(id) == texts_.end()) {
        return L"";
    }

    return std::get<0>(texts_.at(id));
}

void TextRenderer::render(sf::RenderWindow *win) {
    render_text.setFillColor(m_text_color);
    render_text.setOrigin(-position_.x, -position_.y);

    /*
     * std::string for font name
     * 
     * std::vector contains texts' id that match the font
    */
    std::map<std::string, std::vector<std::string>> sorted_text_array;

    for (auto it = texts_.begin(); it != texts_.end(); ++it) {
        sorted_text_array[std::get<1>(it->second)].push_back(it->first);
    }

    for (auto it = sorted_text_array.begin(); it != sorted_text_array.end(); ++it) {
        render_text.setFont(fonts_[it->first]);

        for (auto text_id : it->second) {

            packSFText(text_id, render_text);
            win->draw(render_text);
        }
    }
}

void TextRenderer::alignText(std::string id) {
    if (texts_.find(id) == texts_.end()) {
        return;
    }

    render_text.setFont(fonts_[std::get<1>(texts_.at(id))]);
    packSFText(id, render_text);

    auto textRect = render_text.getLocalBounds();

    std::get<5>(texts_.at(id)) = -(textRect.left + textRect.width / 2.0f);
    std::get<6>(texts_.at(id)) = -(textRect.top + textRect.height / 2.0f);
}

void TextRenderer::setTextColor(sf::Color color) {

    m_text_color = color;
}
