//
// Created by samsa on 6/20/2020.
//

#ifndef EMCMEETING_AUDIOCAPTURER_HPP
#define EMCMEETING_AUDIOCAPTURER_HPP

#include <SFML/Audio.hpp>

#include "../NetWork/Socket.hpp"
#include "../../../ToolBox/ToolBox.hpp"
#include "../MessagePackage/MessagePackage.hpp"

#include <memory>
#include <thread>
#include <deque>

class AudioCapturer {

    std::deque<Socket::Message> *m_audio_stream_ptr = nullptr;

    float m_audio_chunk_size_sencond, m_record_interval;

    std::string m_audio_recorder_default_drivce = "";
    std::unique_ptr<sf::SoundBufferRecorder> m_recorder;

    bool m_thread_stop = false;
    std::unique_ptr<std::thread> m_audio_recorder_thread;

    void CreateRecorderThread_();
    void AudioRecord_();

public:
    AudioCapturer(std::deque<Socket::Message> *stream_buffer, float record_length, float record_internal = 0.0f);
    ~AudioCapturer();

    void SetDrivce(const std::string drivce);

    bool Start();

    void Stop();
};


#endif //EMCMEETING_AUDIOCAPTURER_HPP
