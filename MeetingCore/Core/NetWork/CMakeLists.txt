add_library(TCPServerlib
            TCPServer.cpp
            TCPServer.hpp)

add_library(TCPClientlib
            TCPClient.cpp
            TCPClient.hpp)

target_link_libraries(TCPClientlib TCPServerlib)
