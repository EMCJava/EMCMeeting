//
// Created by loyusum on 7/6/2020.
//

#include "AccountData.hpp"

AccountData::AccountData() : AccountData("", "") {

}

AccountData::AccountData(std::string user_name, std::string password) : m_user_name(user_name), m_password(password) {

    ToolBox::log() << "Account created with user name [" << m_user_name << "] and password [" << m_password << "]"
                   << std::endl;

}

void AccountData::SetActiveTime(float time) {

    m_active_time = time;

}

void AccountData::AddActiveTime(float time) {

    m_active_time += time;

}

float AccountData::GetFocusTime() {
    return m_active_time;
}
