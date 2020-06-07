//
// Created by loyusum on 7/6/2020.
//

#include "AccountData.hpp"

AccountData::AccountData() : AccountData("", "") {

}

AccountData::AccountData(std::string user_name, std::string password) : m_user_name(user_name), m_password(password) {

    ToolBox::log() << "Account created with user name [" << m_user_name << "] and password [" << m_password << "]" << std::endl;

}

bool AccountData::login() {
    if (m_is_login) {
        return true;
    }

#warning login not finished

    return false;
}

void AccountData::logout() {

#warning logout not finished

    m_is_login = false;

}

void AccountData::SetActiveTime(float time) {

    m_active_time = time;

}

void AccountData::AddActiveTime(float time) {

    m_active_time += time;

}
