//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_CONSTANT_HPP
#define EMCMEETING_CONSTANT_HPP

#include <string>

#define USE_STATIC_IP

namespace Constant{

    constexpr float IDLE_KICK_TIME_SECOND = 120;
    constexpr int SOCKET_PORT = 20546;
    constexpr float SCREEN_SHOT_DELAY = 0.05f;

    static const char positive_message[] = "ACCEPTED";
    static const char negative_message[] = "REJECTED";

    static const char server_disconnect_full[] = "STOP_FULL";
    static const char server_disconnect_idle[] = "STOP_IDLE";

    static constexpr char data_spilt = '\1';
    
    static constexpr char frag_server_login = 'L';
    static constexpr char frag_upload_user_data = 'D';
    static constexpr char frag_user_leave = 'G';
    static constexpr char frag_image = 'I';
    static constexpr char frag_image_jpg_file = 'J';

    static const char path_user_account_data[] = "Account";
    static const char path_tem_image_send_file[] = "resource/tem.jpg";

#ifdef USE_STATIC_IP

    const std::string SOCKET_IP = "127.0.0.1";

#endif

}

#endif //EMCMEETING_CONSTANT_HPP
