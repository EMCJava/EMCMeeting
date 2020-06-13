//
// Created by loyusum on 9/6/2020.
//

#include "UserClient.hpp"

UserClient::UserClient(std::string server_ip, int port) {

    m_tcp_client = ToolBox::make_unique<TCPClient>();

    const auto result = m_tcp_client->connect(std::move(server_ip), port);

    if (result != -1) {

        m_has_connect_server = true;

    }

}

bool UserClient::HasConnect() {

    return m_has_connect_server;

}

bool UserClient::login(AccountData &accountData) {

    std::string account_data_str =
            Constant::frag_server_login + accountData.m_user_name + Constant::data_spilt + accountData.m_password;

    Socket::Message mes;

    mes.mes.assign(account_data_str.begin(), account_data_str.begin() + account_data_str.size());

    // tell server we need to login
    m_tcp_client->send(mes);

    // server tell us whether we successfully login or not
    m_tcp_client->receive(mes);

    std::string recv_message(mes.mes.begin(), mes.mes.end());
    std::string posi_str(Constant::positive_message,
                             Constant::positive_message + sizeof(Constant::positive_message));

    // if message is positive
    if (posi_str == recv_message) {

        accountData.m_is_login = true;

        return true;

    }

    accountData.m_is_login = false;
    return false;
}

void UserClient::logout() {

#warning logout not finished

}

void UserClient::SendAccountDataToServer_(AccountData &accountData) {

    std::string attention_data = DataCollector::GenMessageData(accountData);

    std::string send_data_str = Constant::frag_upload_user_data + attention_data;

    Socket::Message mes;

    mes.mes.assign(send_data_str.begin(), send_data_str.end());

    // send attention message to server
    m_tcp_client->send(mes);
}
