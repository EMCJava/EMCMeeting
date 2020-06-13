//
// Created by loyusum on 8/6/2020.
//

#ifndef EMCMEETING_DATACOLLECTOR_HPP
#define EMCMEETING_DATACOLLECTOR_HPP

#include "../../AccountData/AccountData.hpp"
#include "../../../../ToolBox/Constant.hpp"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip> // for std::setprecision

class DataCollector {

public:
    DataCollector();

    struct AttentionData {

        std::string name;
        float active_time_percentage = 0.0f;

    };

    // [user name, data]
    std::map<std::string, AttentionData> m_clients_data;

    static std::string GenMessageData(AccountData accountData);

    static AttentionData DecodeMessageData(std::string data);
};


#endif //EMCMEETING_DATACOLLECTOR_HPP
