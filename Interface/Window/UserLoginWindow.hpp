//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_USERLOGINWINDOW_HPP
#define EMCMEETING_USERLOGINWINDOW_HPP

#include <ctime> //for clock()
#include <string>
#include <SFML/Graphics.hpp>

#include "WindowBase.hpp"
#include "../../ToolBox/ToolBox.hpp"

#include "../UI/Button/Button.hpp"
#include "../UI/InputField/InputField.hpp"

class UserLoginWindow : public WindowBase {

private:

    static const std::string sm_font_path;


    std::vector<InputField> m_input_fields;
    std::unique_ptr<Button> m_buttons;

    void text_input(const char character);

public:
    std::string user_name = "Empty", password = "Empty";

    UserLoginWindow(int screen_size_x, int screen_size_y);

    // return false if window has closed or user had completed to fill in their data
    bool Update() override;

    void Close() override ;
};


#endif //EMCMEETING_USERLOGINWINDOW_HPP
