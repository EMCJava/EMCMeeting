//
// Created by loyusum on 12/6/2020.
//

#ifndef EMCMEETING_USERDATABASE_HPP
#define EMCMEETING_USERDATABASE_HPP

#include "../../../../ToolBox/ToolBox.hpp"

#include <sys/stat.h>

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

class UserDataBase {
private:

    std::unique_ptr<std::vector<std::string>> m_user_black_list;
    std::unique_ptr<std::vector<std::string>> m_user_white_list;
    std::unique_ptr<std::map<std::string, std::string>> m_user_account;

    /*
     *
     *  Read user account data from a file
     *
     */

    void ReadFile_(const std::string &database_file_path);

    /*
     *
     *  Read user account data from a directory
     *
     */
    void ReadDir_(const std::string &database_directory_path);

    /*
     *
     *  Read user account data from a directory or a file
     *
     */
    void Read_(std::string database_path);

public:

    explicit UserDataBase(std::string database_path);

    bool UserAllowLogin(std::string user_name, std::string password);

};


#endif //EMCMEETING_USERDATABASE_HPP
