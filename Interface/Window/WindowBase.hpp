//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_WINDOWBASE_HPP
#define EMCMEETING_WINDOWBASE_HPP

#include <SFML/Graphics.hpp>

class WindowBase {

protected:
    // the size of the main window
    int m_app_size_x, m_app_size_y;
    std::unique_ptr<sf::RenderWindow> m_app;

    // adjust any property the relate to window size
    virtual void ChangeWindowsSize_() {};

public:

    virtual bool Update() = 0;

};

#endif //EMCMEETING_WINDOWBASE_HPP
