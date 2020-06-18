add_library(MessagePackagelib
        MessagePackage.cpp
        MessagePackage.hpp)

if (UNIX)
    target_include_directories(MessagePackagelib PUBLIC ${SFML_INCLUDE_DIRS})
endif ()