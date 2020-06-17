//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_WINDOW_HPP
#define EMCMEETING_WINDOW_HPP

#include <iostream>
#include <deque>
#include <SFML/Graphics.hpp>

// for screen shot fps
#include "../../ToolBox/Constant.hpp"

#include "WindowBase.hpp"

//for make_unique
#include "../../ToolBox/ToolBox.hpp"

#include "../UI/ImageRenderer/ImageRenderer.hpp"

#include <chrono>

class EMCWindow : public WindowBase{

public:
    /*
 *
 *  Image Buffer
 *
 */

    struct ImageBuffer{

        sf::Image im;
        float time_since_start;
    };


private:

    // adjust any property the relate to window size
    void ChangeWindowsSize_() override ;

    std::unique_ptr<ImageRenderer> m_image_renderer;

    typeof(std::chrono::system_clock::now()) m_streaming_begin_time;
    std::deque<ImageBuffer> m_image_buffer;

    bool m_is_focusing = true;

public:

    EMCWindow(bool fullscreen, int screen_size_x = 800, int screen_size_y = 600);

    // return true if the current main window is running
    bool IsOpen() const;

    // return true if the current main window is running
    bool Update() override ;

    bool IsOnFocus();

    void SetTitle(std::string str);

    void SetImage(sf::Image image);

    void PushBackImageBuffer(ImageBuffer&& buffer);
};


#endif //EMCMEETING_WINDOW_HPP
