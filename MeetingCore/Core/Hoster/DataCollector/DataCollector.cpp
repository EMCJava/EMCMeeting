//
// Created by loyusum on 8/6/2020.
//

#include "DataCollector.hpp"

DataCollector::DataCollector() {


}

std::string DataCollector::GenMessageData(AccountData accountData) {

    const auto time_passed = (float) std::chrono::duration_cast<std::chrono::microseconds>(
            accountData.m_end_meeting_time - accountData.m_begin_meeting_time).count() *
                             std::chrono::microseconds::period::num /
                             std::chrono::microseconds::period::den;

    auto active_percentage = accountData.m_active_time / time_passed;

    std::stringstream active_percentage_result;

    active_percentage_result << std::setprecision(4) << active_percentage;

    return accountData.m_user_name + Constant::data_spilt + active_percentage_result.str();

}

DataCollector::AttentionData DataCollector::DecodeMessageData(std::string data) {

    std::vector<std::string> result_arr;
    AttentionData result;

    std::stringstream ss(data);
    std::string token;
    while (std::getline(ss, token, Constant::data_spilt)) {
        result_arr.push_back(token);
    }

    result.name = result_arr[0];
    result.active_time_percentage = (float) atof(result_arr[1].c_str());

    return result;
}
