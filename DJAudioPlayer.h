/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 15 Jul 2022 3:52:41pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class DJAudioPlayer : public AudioSource
{
    public:
         DJAudioPlayer(AudioFormatManager& formatManager);
        ~DJAudioPlayer();

        // ==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);

        /** get the relative position of the playhead */
        double getPositionRelative();

        /** set the reverb controls */
        void setRoomSizeLevel(float roomSizeValue);
        void setDampLevel(float dampAmt);
        void setDryLevel(float dryValue);
        void setWetLevel(float wetValue);

        void start(); // start the track
        void pause(); // pause the track
        void stop();  // get track to stop at start position
        void loop();  // loop the audio track

        double getDurationInSeconds(); // get track duration in seconds

    private:
        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

        ReverbAudioSource reverbSrc{ &resampleSource, false };
        Reverb::Parameters reverbParams;
};