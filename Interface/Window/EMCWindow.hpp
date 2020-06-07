//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_WINDOW_HPP
#define EMCMEETING_WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

//for make_unique
#include "../../ToolBox/ToolBox.hpp"

class Window {

private:

    // the size of the main window
    int m_app_size_x, m_app_size_y;
    std::unique_ptr<sf::RenderWindow> m_app;

    // adjust any property the relate to window size
    void ChangeWindowsSize_();

public:

    Window(bool fullscreen, int screen_size_x = 800, int screen_size_y = 600);

    // return true if the current main window is running
    bool IsOpen() const;

    void Update();
};


#endif //EMCMEETING_WINDOW_HPP
