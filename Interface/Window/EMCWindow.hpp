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

class Window : public WindowBase{

private:

    // adjust any property the relate to window size
    void ChangeWindowsSize_() override ;

    bool m_is_focusing = true;

public:

    Window(bool fullscreen, int screen_size_x = 800, int screen_size_y = 600);

    // return true if the current main window is running
    bool IsOpen() const;

    // return true if the current main window is running
    bool Update() override ;

    bool IsOnFocus();

    void SetTitle(std::string str);
};


#endif //EMCMEETING_WINDOW_HPP
