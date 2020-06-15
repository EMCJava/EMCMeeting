//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_WINDOW_HPP
#define EMCMEETING_WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "WindowBase.hpp"

//for make_unique
#include "../../ToolBox/ToolBox.hpp"

#include "../UI/ImageRenderer/ImageRenderer.hpp"

#include <chrono>

class EMCWindow : public WindowBase{

private:

    // adjust any property the relate to window size
    void ChangeWindowsSize_() override ;

    std::unique_ptr<ImageRenderer> m_image_renderer;

    bool m_is_focusing = true;

public:

    EMCWindow(bool fullscreen, int screen_size_x = 800, int screen_size_y = 600);

    // return true if the current main window is running
    bool IsOpen() const;

    // return true if the current main window is running
    bool Update() override ;

    bool IsOnFocus();

    void SetTitle(std::string str);

    void SetImage(sf::Image image);
};


#endif //EMCMEETING_WINDOW_HPP
