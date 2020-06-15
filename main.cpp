#include "MeetingCore/EMCMeeting.hpp"

#include <chrono>

#define time_use(x) {\
    auto start_time = std::chrono::system_clock::now();\
    x;\
    std::cout << "Time used " << ((double) std::chrono::duration_cast<std::chrono::microseconds>(\
    std::chrono::system_clock::now() - start_time).count() *\
    std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << std::endl;\
}

int main() {

    EMCMeeting met(10);

    int frame_count = 0;
    auto begin_time = std::chrono::system_clock::now();
    while (met.Update()) {

        if (++frame_count > 1000) {

            const auto time_passed = (float) std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now() - begin_time).count() *
                                     std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

            std::cout << "fps : " << frame_count / time_passed << std::endl;

            begin_time = std::chrono::system_clock::now();
            frame_count = 0;
        }

    }

    return 0;
}
