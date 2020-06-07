add_library(tcp_server
            TCPServer.cpp
            TCPServer.hpp)

add_library(tcp_client
            TCPClient.cpp
            TCPClient.hpp)

target_link_libraries(tcp_client tcp_server)
