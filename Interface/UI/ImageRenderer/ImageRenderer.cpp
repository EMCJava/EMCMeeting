//
// Created by loyusum on 14/6/2020.
//

#include "ImageRenderer.hpp"

ImageRenderer::ImageRenderer(sf::Vector2f position, sf::Vector2f size) : UI(position), m_image_size(size) {
    ImageRenderer::setPosition(position);

    m_image_tx.create((int) size.x, (int) size.y);
    m_image_tx.clear(sf::Color::White);
    m_image_tx.display();
    
    m_image_sp = ToolBox::make_unique<sf::Sprite>(m_image_tx.getTexture());
    m_image_sp->setPosition(position_);
}

void ImageRenderer::render(sf::RenderWindow *win) {
    if (m_image_has_change) {

        m_image_sp = ToolBox::make_unique<sf::Sprite>(m_image_tx.getTexture());
        m_image_sp->setPosition(position_);

        m_image_has_change = false;
    }

    win->draw(*m_image_sp);
}

void ImageRenderer::SetImage(sf::Image &image) {
    m_image_has_change = true;

    static sf::Texture image_tx;
    image_tx.loadFromImage(image);
    sf::Sprite image_sp(image_tx);

    // fit window size
    const auto image_size = image.getSize();
    image_sp.scale(m_image_size.x / image_size.x, m_image_size.y / image_size.y);

    m_image_tx.clear(sf::Color::Transparent);
    m_image_tx.draw(image_sp);
    m_image_tx.display();
}

void ImageRenderer::setPosition(sf::Vector2f position) {
    UI::setPosition(position);
}

void ImageRenderer::ReSize(sf::Vector2f size) {
    m_image_has_change = true;
    m_image_size = size;

    static sf::Texture image_tx = m_image_tx.getTexture();
    sf::Sprite image_sp(image_tx);

    m_image_tx.create((int) size.x, (int) size.y);

    const auto image_size = image_tx.getSize();
    image_sp.scale(m_image_size.x / image_size.x, m_image_size.y / image_size.y);

    m_image_tx.clear(sf::Color::Transparent);
    m_image_tx.draw(image_sp);
    m_image_tx.display();
}
