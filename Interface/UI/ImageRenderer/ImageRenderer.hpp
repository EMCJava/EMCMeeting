//
// Created by loyusum on 14/6/2020.
//

#ifndef EMCMEETING_IMAGERENDERER_HPP
#define EMCMEETING_IMAGERENDERER_HPP

#include "../../../ToolBox/ToolBox.hpp"

#include <memory>

#include <SFML/Graphics.hpp>
#include "../UI.hpp"

class ImageRenderer : UI {

private:
    bool m_image_has_change = true;

    sf::RenderTexture m_image_tx;
    std::unique_ptr<sf::Sprite> m_image_sp;

    sf::Vector2f m_image_size;

public:
    ImageRenderer(sf::Vector2f position, sf::Vector2f size);

    void render(sf::RenderWindow *win);

    void SetImage(sf::Image &image);

    void ReSize(sf::Vector2f size);

    void setPosition(sf::Vector2f position) override;
};


#endif //EMCMEETING_IMAGERENDERER_HPP
