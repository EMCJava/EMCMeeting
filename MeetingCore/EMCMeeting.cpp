//
// Created by loyusum on 7/6/2020.
//

#include "EMCMeeting.hpp"

EMCMeeting::EMCMeeting() {

    ToolBox::log() << "new EMCMeeting Created" << std::endl;

    m_main_window = ToolBox::make_unique<Window>(false, 800, 600);

#warning It should implement in UI

    std::string username, password;
    std::cout << "Enter your user name : ";
    std::getline(std::cin, username);
    std::cout << "Enter your password : ";
    std::getline(std::cin, password);

    m_user_account_data = ToolBox::make_unique<AccountData>(username, password);
}

bool EMCMeeting::Update() {

    if (m_main_window->IsOpen()) {

        m_main_window->Update();

    } else {
#warning[return false;] commented for testing without window
        //ToolBox::err() << "Window was closed, unable to update" << std::endl;
        //return false;
    }

    return true;

}
