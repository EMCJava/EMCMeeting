//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_EMCMEETING_HPP
#define EMCMEETING_EMCMEETING_HPP

#include <iostream>
#include <memory>

#include "../Interface/Window/EMCWindow.hpp"
#include "Core/AccountData/AccountData.hpp"

//for make_unique
#include "../ToolBox/ToolBox.hpp"

class EMCMeeting {

private:

    std::unique_ptr<Window> m_main_window;
    std::unique_ptr<AccountData> m_user_account_data;

public:

    EMCMeeting();

    // return true if every things work well
    bool Update();

};


#endif //EMCMEETING_EMCMEETING_HPP
