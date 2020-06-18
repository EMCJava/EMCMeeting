cmake_minimum_required(VERSION 3.16)
project(EMCMeeting)

set(CMAKE_CXX_STANDARD 11)

if (STATIC_BUILD)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif ()

if (UNIX)
    message("${CMAKE_CURRENT_SOURCE_DIR}/cmake-module")
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-module") # Tell CMake where to find the module
endif ()

find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)
# include_directories(PUBLIC "C:/SFML/SFML-2.5.1-g++/include")

if (WIN32)
    set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
    message("sfml include path : " ${SFML_INCLUDE_DIR})

endif ()

if (UNIT_TEST)
    add_executable(EMCMeeting unit_test/Main.cpp)
    add_subdirectory(MeetingCore/Core/ScreenShot)
    target_link_libraries(EMCMeeting ScreenShotlib)
    # target_link_libraries(EMCMeeting -static-libgcc -static-libstdc++)
else ()

    add_subdirectory(Interface)
    add_subdirectory(MeetingCore)

    target_link_libraries(UserAskWhetherHosterWindowlib sfml-graphics)
    target_link_libraries(ErrorWindowlib sfml-graphics)
    target_link_libraries(UIlib sfml-graphics)
    target_link_libraries(MessagePackagelib PUBLIC sfml-graphics)
    target_link_libraries(UserClientlib sfml-graphics)

    target_link_libraries(EMCWindowlib ImageRendererlib)
    target_link_libraries(UserLoginWindowlib Buttonlib)
    target_link_libraries(UserLoginWindowlib InputFieldlib)

    target_link_libraries(UserClientAskHosterIPAndPortWindowlib sfml-graphics opengl32 winmm gdi32)

    target_link_libraries(ScreenShotlib sfml-graphics opengl32 winmm gdi32)

    target_link_libraries(HosterServerlib TCPServerlib)
    target_link_libraries(HosterServerlib UserDataBaselib)
    target_link_libraries(HosterServerlib MessagePackagelib)
    target_link_libraries(HosterServerlib ScreenShotlib)

    target_link_libraries(UserClientlib TCPClientlib)

    target_link_libraries(EMCWindowlib sfml-graphics opengl32 winmm gdi32)
    target_link_libraries(EMCMeetinglib EMCWindowlib)
    target_link_libraries(EMCMeetinglib AccountDatalib)
    target_link_libraries(EMCMeetinglib UserLoginWindowlib)
    target_link_libraries(EMCMeetinglib UserAskWhetherHosterWindowlib)
    target_link_libraries(EMCMeetinglib UserClientAskHosterIPAndPortWindowlib)
    target_link_libraries(EMCMeetinglib HosterServerlib)
    target_link_libraries(EMCMeetinglib DataCollectorlib)
    target_link_libraries(EMCMeetinglib ErrorWindowlib)
    target_link_libraries(EMCMeetinglib UserClientlib)

    # Main executable file
    add_executable(EMCMeeting main.cpp)
    target_link_libraries(EMCMeeting EMCMeetinglib)
    target_link_libraries(EMCMeeting -static-libgcc -static-libstdc++)

endif ()