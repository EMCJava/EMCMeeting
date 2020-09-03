//
// Created by samsa on 6/20/2020.
//

#include "AudioCapturer.hpp"

AudioCapturer::AudioCapturer(std::deque<Socket::Message> *stream_buffer, float record_length, float record_interval)
        : m_audio_stream_ptr(stream_buffer), m_audio_chunk_size_sencond(record_length),
          m_record_interval(record_interval) {


}

AudioCapturer::~AudioCapturer() {

    if (m_audio_recorder_thread) {

        m_thread_stop = true;

        // if the thread is still running
        if (m_audio_recorder_thread->joinable()) {

            // wait till the thread stop
            m_audio_recorder_thread->join();
        }

        m_audio_recorder_thread.reset();
    }
}

void AudioCapturer::AudioRecord_() {

    if (!sf::SoundBufferRecorder::isAvailable()) {

        ToolBox::err() << "System does not support audio recording." << std::endl;
        return;
    }

    std::string target_drive = m_audio_recorder_default_drivce;

    //goto skip__;

    // user didn't specify a drive
    if (m_audio_recorder_default_drivce.empty()) {
        // get all drivces that suppose to be available
        const auto drive_list = sf::SoundRecorder::getAvailableDevices();

        if (drive_list.empty()) {

            ToolBox::err() << "No available devices to record." << std::endl;
            return;
        }

        // we get the first device that available
        target_drive = drive_list[0];
    }

    if(!m_recorder)
        m_recorder = ToolBox::make_unique<sf::SoundBufferRecorder>();

    // set the record drivce
    if (!m_recorder->setDevice(target_drive)) {

        ToolBox::err() << "drivce " << target_drive << " not available." << std::endl;
        return;
    }

    //skip__:

    while (!m_thread_stop) {

        // previous sound buffer will be deleted when start
        //m_recorder->start();
        //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_audio_chunk_size_sencond * 1000)));
        //m_recorder->stop();

        sf::SoundBuffer buffer;
        buffer.loadFromFile("resource/summertime.ogg");

        // gen message to send format
        Socket::Message mes;
        //MessagePackage::GenMessage(mes, m_recorder->getBuffer());
        MessagePackage::GenMessage(mes, buffer);

        // puch back to audio send queue
        m_audio_stream_ptr->emplace_back(std::move(mes));

        // wait to the specified interval
        // they didn't support float second
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_record_interval * 1000)));
    }
}

void AudioCapturer::SetDrivce(std::string drivce) {

    m_audio_recorder_default_drivce = std::move(drivce);

}

bool AudioCapturer::Start() {

    // if there is a thread recording
    if (m_audio_recorder_thread) {

        // if thread is already stopped
        if (!m_audio_recorder_thread->joinable()) {

            CreateRecorderThread_();

            return true;
        }

        return false;
    } else {

        CreateRecorderThread_();

        return true;
    }

}


void AudioCapturer::Stop() {

    // if there is a thread recording
    if (m_audio_recorder_thread) {

        m_thread_stop = true;

        // if the thread ended
        if (!m_audio_recorder_thread->joinable()) {

            m_audio_recorder_thread.reset();
        }
    }
}

void AudioCapturer::CreateRecorderThread_() {

    m_audio_recorder_thread = ToolBox::make_unique<std::thread>(&AudioCapturer::AudioRecord_, this);
}
