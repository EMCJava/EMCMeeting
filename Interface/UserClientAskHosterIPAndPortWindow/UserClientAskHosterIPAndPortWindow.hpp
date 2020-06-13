//
// Created by loyusum on 9/6/2020.
//

#ifndef EMCMEETING_USERCLIENTASKHOSTERIPANDPORTWINDOW_HPP
#define EMCMEETING_USERCLIENTASKHOSTERIPANDPORTWINDOW_HPP

#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

// check ip
#include "../../MeetingCore/Core/NetWork/Socket.hpp"

#include "../UI/TextRenderer/TextRenderer.hpp"
#include "../UI/Button/Button.hpp"
#include "../UI/InputField/InputField.hpp"
#include "../Window/WindowBase.hpp"
#include "../../ToolBox/ToolBox.hpp"

class UserClientAskHosterIPAndPortWindow : public WindowBase{

private:

    static const std::string sm_font_path;

    std::vector<InputField> m_input_fields;
    std::unique_ptr<Button> m_buttons;
    std::unique_ptr<TextRenderer> m_texts;

    void text_input(const char character);

    bool m_is_cancel = true;

public:
    std::string ip = "0.0.0.0", port = "0";

    UserClientAskHosterIPAndPortWindow(int screen_size_x, int screen_size_y);

    // return false if window has closed or user had completed to fill in their data
    bool Update() override;

    void Close() override ;

    bool IsCancel();

};


#endif //EMCMEETING_USERCLIENTASKHOSTERIPANDPORTWINDOW_HPP
