//
// Created by loyusum on 7/6/2020.
//

#include "EMCMeeting.hpp"

EMCMeeting::EMCMeeting() {

    std::cout << "new EMCMeeting Created" << std::endl;

    m_main_window = ToolBox::make_unique<Window>(false, 800, 600);

#warning It should implement in UI
    int

}

bool EMCMeeting::Update() {

    if (m_main_window->IsOpen()) {

        m_main_window->Update();

    } else {
#warning [return false;] commented for testing without window
        //std::cerr << "Window was closed, unable to update" << std::endl;
        //return false;
    }

    return true;

}
