//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_TOOLBOX_HPP
#define EMCMEETING_TOOLBOX_HPP

#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#if __cplusplus == 201402L || __cplusplus == 201703L

#include <filesystem>

#else

#include <dirent.h>

#endif

namespace ToolBox {

    static std::ostream &log() {
        return std::cout;
    }

    static std::ostream &err() {
        return std::cerr;
    }

    static std::vector<std::string> spilt(std::string str, char sp = ' ') {

        std::vector<std::string> result;

        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, sp)) {
            result.push_back(token);
        }

        return std::move(result);
    }


    static std::vector<std::string> GetFileList(std::string path) {

        std::vector<std::string> result;

#if __cplusplus == 201402L || __cplusplus == 201703L

        for (const auto &entry : std::filesystem::directory_iterator(path))
            result.push_back(entry.path());
#else

        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(path.c_str())) != NULL) {

            /* get all the files and directories within directory */
            while ((ent = readdir(dir)) != NULL) {

                result.push_back(ent->d_name);

            }

            closedir(dir);

        } else {
            err() << "Could not open directory" << std::endl;
        }
#endif

        return std::move(result);
    }


    template<typename T, typename... Ts>
    static std::unique_ptr<T> make_unique(Ts &&... params) {
        return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
    }


};


#endif //EMCMEETING_TOOLBOX_HPP
