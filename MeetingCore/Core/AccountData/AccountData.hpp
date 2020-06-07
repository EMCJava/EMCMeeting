//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_ACCOUNTDATA_HPP
#define EMCMEETING_ACCOUNTDATA_HPP

#include <iostream>
#include <string>

#include "../../../ToolBox/ToolBox.hpp"

class EMCMeeting;

class AccountData {

private:

    std::string m_user_name;
    std::string m_password;

    float m_active_time = 0.0f;

    bool m_is_login = false;

    void SetActiveTime(float time = 0);
    void AddActiveTime(float time);

public:

    AccountData(std::string user_name, std::string password);
    AccountData();

    bool login();
    void logout();

    friend class EMCMeeting;
};


#endif //EMCMEETING_ACCOUNTDATA_HPP
