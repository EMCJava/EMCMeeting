//
// Created by loyusum on 9/6/2020.
//

#ifndef EMCMEETING_ERRORWINDOW_HPP
#define EMCMEETING_ERRORWINDOW_HPP

#include <string>
#include "../Window/WindowBase.hpp"

#include "../../ToolBox/ToolBox.hpp"
#include "../UI/TextRenderer/TextRenderer.hpp"

class ErrorWindow : public WindowBase {

private:

    static const std::string sm_font_path;

    std::unique_ptr<TextRenderer> m_texts;

public:
    ErrorWindow(int screen_size_x, int screen_size_y, std::string error_mes);

    // return false if window has closed or user had completed to fill in their data
    bool Update() override;

    void Close() override ;
};


#endif //EMCMEETING_ERRORWINDOW_HPP
