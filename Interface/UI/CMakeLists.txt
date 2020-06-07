add_subdirectory(Button)
add_subdirectory(InputField)
add_subdirectory(TextRenderer)

add_library(UIlib
            UI.cpp
            UI.hpp)

target_link_libraries(TextRendererlib UIlib)

target_link_libraries(Buttonlib TextRendererlib)

target_link_libraries(InputFieldlib Buttonlib)

