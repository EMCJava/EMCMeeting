//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_USERLOGINWINDOW_HPP
#define EMCMEETING_USERLOGINWINDOW_HPP

#include <string>
#include <SFML/Graphics.hpp>

#include "WindowBase.hpp"
#include "../../ToolBox/ToolBox.hpp"

class UserLoginWindow : public WindowBase {

public:
    std::string user_name, password;

    UserLoginWindow(int screen_size_x, int screen_size_y);

    // return false if window has closed or user had completed to fill in their data
    bool Update() override;
};


#endif //EMCMEETING_USERLOGINWINDOW_HPP
