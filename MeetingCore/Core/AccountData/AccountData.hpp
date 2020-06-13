//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_ACCOUNTDATA_HPP
#define EMCMEETING_ACCOUNTDATA_HPP

#include <iostream>
#include <string>
#include <chrono>

#include "../../../ToolBox/ToolBox.hpp"

class EMCMeeting;

class DataCollector;

class UserClient;

class AccountData {

private:

    std::string m_user_name;
    std::string m_password;

    typeof(std::chrono::system_clock::now()) m_begin_meeting_time, m_end_meeting_time;

    /*
     *
     *  total time that the main window stay active ( concentrated ) since meeting start
     *
     */
    float m_active_time = 0.0f;

    bool m_is_login = false;

    void SetActiveTime(float time = 0);

    void AddActiveTime(float time);

public:

    AccountData(std::string user_name, std::string password);

    AccountData();

    float GetFocusTime();

    friend class EMCMeeting;

    friend class DataCollector;

    friend class UserClient;
};


#endif //EMCMEETING_ACCOUNTDATA_HPP
