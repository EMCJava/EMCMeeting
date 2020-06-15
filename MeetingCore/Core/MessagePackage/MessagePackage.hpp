//
// Created by loyusum on 14/6/2020.
//

#ifndef EMCMEETING_MESSAGEPACKAGE_HPP
#define EMCMEETING_MESSAGEPACKAGE_HPP

#include <fstream>

#include <SFML/Graphics.hpp>
#include "../NetWork/Socket.hpp"

#define SEND_FILE_SIZE false

class MessagePackage {

public:

    using FileSize = int;

    static void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage);
    static void ReadImage(Socket::Message& mes, sf::Image& image);
    static void GenMessage(Socket::Message& mes, sf::Image& image);

    static void GenMessage(Socket::Message& mes, std::string input_file_name);
    static void ReadFile(Socket::Message& mes, std::string output_file_name);
};


#endif //EMCMEETING_MESSAGEPACKAGE_HPP
