//
// Created by loyusum on 7/6/2020.
//

#include "EMCMeeting.hpp"

EMCMeeting::EMCMeeting() {

    ToolBox::log() << "new EMCMeeting Created" << std::endl;

    AccountLogIn();

    m_main_window = ToolBox::make_unique<Window>(false, 800, 600);

}

bool EMCMeeting::Update() {

    if (!m_main_window->Update()) {
#warning[return false;] commented for testing without window
        //ToolBox::err() << "Window was closed, unable to update" << std::endl;
        //return false;
    }

    return true;

}

bool EMCMeeting::AccountLogIn() {
#warning It should implement in UI

    UserLoginWindow loginWindow(400, 200);

    // wait till window had closed, which means user has finished filling in their user data
    while (loginWindow.Update());

    // get the name and password from the input window class
    std::string username = loginWindow.user_name, password = loginWindow.password;

    m_user_account_data = ToolBox::make_unique<AccountData>(username, password);

    // trt to log in
    if (!m_user_account_data->login()) {

        ToolBox::err() << "Invalid user name or password, please check" << std::endl;

        return true;
    }

    return true;
}
