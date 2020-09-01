//
// Created by loyusum on 14/6/2020.
//

#ifndef EMCMEETING_MESSAGEPACKAGE_HPP
#define EMCMEETING_MESSAGEPACKAGE_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <fstream>

#include "../NetWork/Socket.hpp"
#include "../../../ToolBox/ToolBox.hpp"

#define SEND_FILE_SIZE false

class MessagePackage {

public:

    using FileSize = int;

    static void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage);\

    // Image
    static void GenMessage(Socket::Message& mes, sf::Image& image);
    static void ReadImage(Socket::Message& mes, sf::Image& image);
    static void ReadImage_FileStream(Socket::Message& mes, sf::Image& image);

    // Image file
    static void GenMessage(Socket::Message& mes, std::string input_file_name);
    static void ReadFile(Socket::Message& mes, std::string output_file_name);

    // Sound buffer
    static void GenMessage(Socket::Message& mes, const sf::SoundBuffer& buffer);
    static void ReadSoundBuffer(Socket::Message& mes, sf::SoundBuffer& buffer);
};


#endif //EMCMEETING_MESSAGEPACKAGE_HPP
