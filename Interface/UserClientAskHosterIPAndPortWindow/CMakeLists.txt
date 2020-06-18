add_library(UserClientAskHosterIPAndPortWindowlib
            UserClientAskHosterIPAndPortWindow.cpp
            UserClientAskHosterIPAndPortWindow.hpp)

if (UNIX)
    target_include_directories(UserClientAskHosterIPAndPortWindowlib PUBLIC ${SFML_INCLUDE_DIRS})
endif ()