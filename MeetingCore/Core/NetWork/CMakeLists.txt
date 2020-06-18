add_library(TCPServerlib
            TCPServer.cpp
            TCPServer.hpp)

add_library(TCPClientlib
            TCPClient.cpp
            TCPClient.hpp)

if(WIN32)

    target_link_libraries(TCPServerlib Ws2_32)

endif()

target_link_libraries(TCPClientlib TCPServerlib)