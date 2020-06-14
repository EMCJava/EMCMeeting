//
// Created by loyusum on 12/6/2020.
//

#include "UserDataBase.hpp"

#include <utility>

UserDataBase::UserDataBase(std::string database_path) {

    Read_(std::move(database_path));

}

void UserDataBase::Read_(std::string database_path) {

    struct stat st{};
    if (stat(database_path.c_str(), &st) == 0) {
        if (st.st_mode & S_IFDIR) {
            //it's a directory

            ReadDir_(database_path);

        } else if (st.st_mode & S_IFREG) {
            //it's a file

            ReadFile_(database_path);

        } else {
            //something else, not reading anything
            return;
        }
    } else {
        //error, not reading any thing
        return;
    }

}

void UserDataBase::ReadDir_(const std::string &database_directory_path) {

    const std::vector<std::string> file_arr = ToolBox::GetFileList(database_directory_path);

    for (auto &path : file_arr) {

        if (path == "." || path == "..")
            continue;

        // read all the files and directories within directory
        Read_(database_directory_path + "/" + path);

    }

}

void UserDataBase::ReadFile_(const std::string &database_file_path) {

    std::ifstream database_input_file(database_file_path);

    if (!database_input_file) {

        ToolBox::err() << "Can't open database file !!" << std::endl;
        return;

    }

    std::string file_line;
    std::vector<std::string> file_lines;
    while (std::getline(database_input_file, file_line)) {
        file_lines.emplace_back(std::move(file_line));
    }

    if (file_lines[0] == "user account") {

        if (!m_user_account) {

            m_user_account = ToolBox::make_unique<std::map<std::string, std::string>>();

        }

        // load data
        for (int i = 1; i < file_lines.size(); ++i) {

            std::vector<std::string> arr = ToolBox::spilt(file_lines[i], ' ');

            std::string user_name = arr[0], password;

            // might contain space
            if (arr.size() > 2) {

                if (user_name[0] == '\'' || user_name[0] == '\"') {

                    int j = 1;
                    for (; j < arr.size(); ++j) {
                        user_name += ' ' + arr[j];


                        if (arr[j].back() == '\'' || arr[j].back() == '\"')
                            break;
                    }

                    //remove '\'' or '\"' from beginning
                    user_name = user_name.substr(1);

                    // actually found string that ends with '\'' or '\"'
                    if (j != arr.size()) {
                        user_name.pop_back();
                    }

                    //remove user name
                    arr.erase(arr.begin(), arr.begin() + j + 1);
                }

            } else {

                //remove user name
                arr.erase(arr.begin());

            }

            // if no password
            if (arr.empty()) {

                continue; // not recorded
            }

            password = arr[0];

            // might contain space
            if (arr.size() > 2) {

                if (password[0] == '\'' || password[0] == '\"') {

                    int j = 1;
                    for (; j < arr.size(); ++j) {
                        password += ' ' + arr[j];


                        if (arr[j].back() == '\'' || arr[j].back() == '\"')
                            break;
                    }

                    //remove '\'' or '\"' from beginning
                    password = password.substr(1);

                    // actually found string that ends with '\'' or '\"'
                    if (j != arr.size()) {
                        password.pop_back();
                    }
                }
            }

            // save data
            (*m_user_account)[user_name] = password;
        }

    } else if (file_lines[0] == "white list") {

        if (!m_user_white_list) {

            m_user_white_list = ToolBox::make_unique<std::vector<std::string>>();

        }

        // load data
        for (int i = 1; i < file_lines.size(); ++i) {

            m_user_white_list->push_back(file_lines[i]);
        }

    } else if (file_lines[0] == "black list") {

        if (!m_user_black_list) {

            m_user_black_list = ToolBox::make_unique<std::vector<std::string>>();

        }

        // load data
        for (int i = 1; i < file_lines.size(); ++i) {

            m_user_black_list->push_back(file_lines[i]);
        }

    } else {

        ToolBox::err() << "unknown database file type !!" << std::endl;

    }

}

bool UserDataBase::UserAllowLogin(std::string user_name, std::string password) {

    // if black list enabled
    if (m_user_black_list) {

        // user exist in black list
        if(std::find(m_user_black_list->begin(), m_user_black_list->end(), user_name) != m_user_black_list->end()){

            return false;

        }

    }

    // if white list enabled
    if (m_user_white_list) {

        // user 'not' exist in white list
        if(std::find(m_user_white_list->begin(), m_user_white_list->end(), user_name) == m_user_white_list->end()){

            return false;

        }

    }

    // check if account exist
    auto it = m_user_account->find(user_name);

    // account not exist
    if(it == m_user_account->end()){

        return false;
    }

    // check if password correct
    return it->second == password;
}
