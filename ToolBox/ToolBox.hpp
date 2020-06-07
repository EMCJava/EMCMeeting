//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_TOOLBOX_HPP
#define EMCMEETING_TOOLBOX_HPP

#include <iostream>
#include <memory>

class ToolBox {

public:

    static std::ostream& log(){
        return std::cout;
    }

    static std::ostream& err(){
        return std::cerr;
    }

#if __cplusplus == 201402L || __cplusplus == 201703L

#define ToolBox std

#else

    template<typename T, typename... Ts>
    static std::unique_ptr<T> make_unique(Ts &&... params) {
        return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
    }

#endif

};



#endif //EMCMEETING_TOOLBOX_HPP
