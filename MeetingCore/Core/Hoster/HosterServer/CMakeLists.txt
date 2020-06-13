add_library(HosterServerlib
            HosterServer.cpp
            HosterServer.hpp)

target_link_libraries(HosterServerlib pthread)