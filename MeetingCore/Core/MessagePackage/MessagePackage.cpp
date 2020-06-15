//
// Created by loyusum on 14/6/2020.
//

#include "MessagePackage.hpp"

void MessagePackage::resizeImage(const sf::Image &originalImage, sf::Image &resizedImage) {
    const sf::Vector2u originalImageSize{originalImage.getSize()};
    const sf::Vector2u resizedImageSize{resizedImage.getSize()};
    for (unsigned int y{0u}; y < resizedImageSize.y; ++y) {
        for (unsigned int x{0u}; x < resizedImageSize.x; ++x) {
            unsigned int origX{
                    static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x)};
            unsigned int origY{
                    static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y)};
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
}

void MessagePackage::GenMessage(Socket::Message &mes, sf::Image &image) {
    static constexpr auto image_axis_size = sizeof(typeof(image.getSize().x));

    const auto image_size = image.getSize();

    // we have r, g, b, a, so *4
    const size_t image_ptr_size = image_size.x * image_size.y * 4;
    mes.mes.assign(reinterpret_cast<const char *>(image.getPixelsPtr()),
                   reinterpret_cast<const char *>(image.getPixelsPtr()) + image_ptr_size);

    std::vector<char> size_mes;
    size_mes.resize(image_axis_size * 2);

    // store image size info
    std::memcpy(size_mes.data(), reinterpret_cast<const char *>(&image_size.x), image_axis_size);
    std::memcpy(size_mes.data() + image_axis_size, reinterpret_cast<const char *>(&image_size.y), image_axis_size);

    // insert image size info in front of message
    mes.mes.insert(mes.mes.begin(), size_mes.begin(), size_mes.end());
}

void MessagePackage::ReadImage(Socket::Message &mes, sf::Image &image) {
    static constexpr auto image_axis_size = sizeof(typeof(image.getSize().x));

    typeof(image.getSize()) image_size;

    // get image size info
    std::memcpy(&image_size.x, reinterpret_cast<const char *>(mes.mes.data()), image_axis_size);
    std::memcpy(&image_size.y, reinterpret_cast<const char *>(mes.mes.data() + image_axis_size), image_axis_size);
    image.create(image_size.x, image_size.y);

    // load image data
    image.create(image_size.x, image_size.y, reinterpret_cast<const sf::Uint8 *>(mes.mes.data() + image_axis_size * 2));
}

void MessagePackage::GenMessage(Socket::Message &mes, std::string input_file_name) {

    mes.mes.clear();

    std::ifstream input_file(input_file_name, std::ios::binary);

    // Can't open file
    if (!input_file) {

        return;
    }

    input_file.seekg(0, std::ios::end);
    const FileSize file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

#if SEND_FILE_SIZE

    // store file size in front of message
    mes.mes.assign(reinterpret_cast<const char *>(&file_size),
                   reinterpret_cast<const char *>(&file_size) + sizeof(FileSize));

    mes.mes.resize(sizeof(FileSize) + file_size);

    input_file.read(mes.mes.data() + sizeof(FileSize), file_size);

#else

    mes.mes.resize(file_size);
    input_file.read(mes.mes.data(), file_size);

#endif

    // failed to read file
    if (!input_file) {
        mes.mes.clear();
    }

    input_file.close();
}

void MessagePackage::ReadFile(Socket::Message &mes, std::string output_file_name) {

    // no message
    if (mes.mes.empty()) {

        return;
    }

    std::ofstream output_file(output_file_name, std::ios::binary);

    // Can't open file
    if (!output_file) {

        return;
    }


#if SEND_FILE_SIZE

    FileSize file_size;

    // get file size from message
    std::memcpy(reinterpret_cast<char *>(&file_size), mes.mes.data(), sizeof(FileSize));

    output_file.write(mes.mes.data() + sizeof(FileSize), file_size);

#else

    output_file.write(mes.mes.data(), mes.mes.size());

#endif

    output_file.close();
}
