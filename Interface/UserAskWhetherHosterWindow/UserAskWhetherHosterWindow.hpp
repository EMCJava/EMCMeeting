//
// Created by loyusum on 9/6/2020.
//

#ifndef EMCMEETING_USERASKWHETHERHOSTERWINDOW_HPP
#define EMCMEETING_USERASKWHETHERHOSTERWINDOW_HPP

#include <string>
#include <vector>
#include <memory>

#include "../../ToolBox/ToolBox.hpp"
#include "../Window/WindowBase.hpp"
#include "../UI/Button/Button.hpp"


class UserAskWhetherHosterWindow : public WindowBase {

public:

    enum class UserType {
        Client, Hoster, None
    };

private:

    static const std::string sm_font_path;

    std::vector<Button> m_buttons;

    UserType m_userType = UserType::None;

public:

    UserAskWhetherHosterWindow(int screen_size_x, int screen_size_y);

    // return false if window has closed or user had completed to fill in their data
    bool Update() override;

    void Close() override;

    UserType GetType();
};


#endif //EMCMEETING_USERASKWHETHERHOSTERWINDOW_HPP
