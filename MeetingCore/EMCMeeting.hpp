//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_EMCMEETING_HPP
#define EMCMEETING_EMCMEETING_HPP

#include <iostream>
#include <chrono> // for system clock
#include <memory>

#include "../Interface/ErrorWindow/ErrorWindow.hpp"
#include "../Interface/UserClientAskHosterIPAndPortWindow/UserClientAskHosterIPAndPortWindow.hpp"
#include "../Interface/UserAskWhetherHosterWindow/UserAskWhetherHosterWindow.hpp"
#include "../Interface/UserLoginWindow/UserLoginWindow.hpp"
#include "../Interface/Window/EMCWindow.hpp"
#include "Core/AccountData/AccountData.hpp"

#include "Core/AccountData/AccountData.hpp"

//network
#include "Core/Hoster/HosterServer/HosterServer.hpp"
#include "Core/Client/UserClient.hpp"

//for make_unique
#include "../ToolBox/ToolBox.hpp"

class UserClient; // allow client to get image

class EMCMeeting {

private:

    std::unique_ptr<Window> m_main_window;
    std::unique_ptr<AccountData> m_user_account_data;

    /*
     *
     * ptr of the HosterServer, if user is not a hoster, stay nullptr
     *
     */

     std::unique_ptr<HosterServer> m_hoster_server;

    /*
    *
    * ptr of the HosterServer, if user is not a hoster, stay nullptr
    *
    */

    std::unique_ptr<UserClient> m_user_client;

    /*
     *
     * create a window for user to enter their name and password
     * return true if successfully login
     *
     */

    bool AccountLogIn_(bool *isCancel = nullptr);

    /*
     *
     * create a window for user to select their user type [hoster / client]
     * return the user type
     *
     */

    UserAskWhetherHosterWindow::UserType SelectUserType_();

    /*
     *
     * create a window for user to select the hoster ip and port
     * return the [ip in string, port in integer]
     *
     */

    std::pair<std::string, int> SelectIpAndPort_(bool *isCancel = nullptr);

    /*
     *
     * create multiple window and process in order to let user login
     * return true if successfully login
     *
     */

    bool ConnectToHoster_(bool *isCancel = nullptr);

    /*
     *
     * save the time the last time the window has the input focus
     * used to send info to hoster
     *
     */

    typeof(std::chrono::system_clock::now()) m_last_time_since_on_focus;

    /*
     *
     * weather the main window has the input focus in last frame
     *
     */

    bool m_window_on_focus = true;

    /*
     *
     * set m_last_time_since_on_focus be std::chrono::system_clock::now()
     *
     */

    void ResetFocusTimer_();

    /*
     *
     *  Create a window to show error message
     *
     */

    void WinError_(std::string message, int weight = 400, int height = 200);

    void UserSetup_(int max_client = 1);

    void UpdateUserAttentionData_(bool window_is_on_focus);

public:

    explicit EMCMeeting(int max_client = 1);

    // return true if every things work well
    bool Update();

    /*
     *
     * Upload the attention data to hoster if use is not
     *
     */
    void UploadUserAttentionData();

    friend class UserClient;
};


#endif //EMCMEETING_EMCMEETING_HPP
