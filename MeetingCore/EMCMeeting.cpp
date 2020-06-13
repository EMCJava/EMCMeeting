//
// Created by loyusum on 7/6/2020.
//

#include "EMCMeeting.hpp"

EMCMeeting::EMCMeeting(int max_client) {

    ToolBox::log() << "new EMCMeeting Created";

    UserSetup_(max_client);

    m_main_window = ToolBox::make_unique<Window>(false, 800, 600);

    // reset the active timer once the window open
    ResetFocusTimer_();

}

bool EMCMeeting::Update() {

    // if user is a hoster of meeting
    if (m_hoster_server) {

        m_hoster_server->Update();

    } else { // otherwise ...

        UpdateUserAttentionData_(m_main_window->IsOnFocus());

    }

    if (!m_main_window->Update()) {
#warning for server debug
        if (m_hoster_server)
            return true;

        ToolBox::err() << "Window was closed, unable to update" << std::endl;

        // if is a client
        if (m_user_client) {

            // upload any required data to hoster and exit
            UploadUserAttentionData();

        }

        // tell user the main window has closed
        return false;
    }

    return true;

}

bool EMCMeeting::AccountLogIn_(bool *isCancel) {
    UserLoginWindow loginWindow(400, 200);

    // wait till window had closed, which means user has finished filling in their user data
    while (loginWindow.Update());

    if(isCancel && loginWindow.IsCancel()){
        *isCancel = true;

        return false;
    }

    // get the name and password from the input window class
    std::string username = loginWindow.user_name, password = loginWindow.password;

    m_user_account_data = ToolBox::make_unique<AccountData>(username, password);
    m_user_account_data->m_begin_meeting_time = std::chrono::system_clock::now();

    // try to log in
    if (!m_user_client->login(*m_user_account_data)) {

        ToolBox::err() << "Invalid user name or password, please check" << std::endl;
        WinError_("Invalid user name or password, please check");

        return false;
    }

    return true;
}

void EMCMeeting::UploadUserAttentionData() {

    // is a client
    if (m_user_client) {

        // user attention timer last accumulate
        UpdateUserAttentionData_(false);

        m_user_account_data->m_end_meeting_time = std::chrono::system_clock::now();
        m_user_client->SendAccountDataToServer_(*m_user_account_data);

    }

}

void EMCMeeting::ResetFocusTimer_() {

    m_last_time_since_on_focus = std::chrono::system_clock::now();

}

void EMCMeeting::UpdateUserAttentionData_(bool window_is_on_focus) {
    if (window_is_on_focus) {

        if (!m_window_on_focus) {

            ResetFocusTimer_();

        }

        m_window_on_focus = true;

    } else {

        if (m_window_on_focus) {

            // time passed since the last time we gain focus
            const auto current_accumulated_focus_time = std::chrono::system_clock::now() - m_last_time_since_on_focus;

            // reset timer
            ResetFocusTimer_();

            // time passed since the last time we gain focus in second (float form)
            const auto time_passed = (float) std::chrono::duration_cast<std::chrono::microseconds>(
                    current_accumulated_focus_time).count() * std::chrono::microseconds::period::num /
                                     std::chrono::microseconds::period::den;

            // accumulate attention time
            m_user_account_data->AddActiveTime(time_passed);
        }

        m_window_on_focus = false;
    }
}

UserAskWhetherHosterWindow::UserType EMCMeeting::SelectUserType_() {

    UserAskWhetherHosterWindow win(400, 200);
    while (win.Update());

    return win.GetType();
}

std::pair<std::string, int> EMCMeeting::SelectIpAndPort_(bool *isCancel) {

    std::unique_ptr<UserClientAskHosterIPAndPortWindow> win;

    bool ip_accept = true;

    do {

        if (!ip_accept) {
            WinError_("Ip address invalid");
        }

        win = ToolBox::make_unique<UserClientAskHosterIPAndPortWindow>(
                400, 200);
        while (win->Update());

        if (isCancel) {

            if (win->IsCancel()) {

                *isCancel = true;
                return {};
            }
        }

        ToolBox::log() << "Connect to ip " << win->ip << ":" << win->port << std::endl;

        ToolBox::log() << "IP " << ((ip_accept = Socket::IsValidIP(win->ip)) ? "valid" : "invalid") << std::endl;

    } while (!ip_accept);

    return {win->ip, atoi(win->port.c_str())};
}

bool EMCMeeting::ConnectToHoster_(bool *isCancel) {

    bool has_connect = true;

    do {

        if (!has_connect) {

            WinError_("Server no response");
        }

        bool is_cancel = false;
        const auto addr = SelectIpAndPort_(&is_cancel);

        if (isCancel && is_cancel) {

            *isCancel = true;
            return false;
        }

        m_user_client = ToolBox::make_unique<UserClient>(std::get<0>(addr), std::get<1>(addr));

        has_connect = m_user_client->HasConnect();
    } while (!has_connect);

    bool is_cancel = false;
    while (!AccountLogIn_(&is_cancel)){
        if(is_cancel) {

            m_user_client.reset();

            // go back to previous stage (enter ip and port)
            return ConnectToHoster_(isCancel);

        }
    }

    return true;
}

void EMCMeeting::WinError_(std::string message, int weight, int height) {

    ErrorWindow e_win(weight, height, message);
    while (e_win.Update());

}

void EMCMeeting::UserSetup_(int max_client) {

    const auto user_type = SelectUserType_();

    if (user_type == UserAskWhetherHosterWindow::UserType::Client) {

        ToolBox::log() << " as a Client" << std::endl;

        bool is_cancel = false;
        ConnectToHoster_(&is_cancel);

        if (is_cancel) {

            // select type again
            UserSetup_();
        }

    } else if (user_type == UserAskWhetherHosterWindow::UserType::Hoster) {

        ToolBox::log() << " as a Hoster" << std::endl;

        m_hoster_server = ToolBox::make_unique<HosterServer>(max_client);
        m_hoster_server->Start();

    } else {

        ToolBox::log() << std::endl << "No User Type selected, terminating process" << std::endl;
        exit(0);

    }

}
