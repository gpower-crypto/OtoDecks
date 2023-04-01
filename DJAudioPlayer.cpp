/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 15 Jul 2022 3:52:41pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager) 
{
    reverbParams.roomSize = 0;
    reverbParams.damping = 0;
    reverbParams.wetLevel = 0;
    reverbParams.dryLevel = 1.0;

    reverbSrc.setParameters(reverbParams);
}

DJAudioPlayer::~DJAudioPlayer()
{

}

// ==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(
        samplesPerBlockExpected,
        sampleRate);
    resampleSource.prepareToPlay(
        samplesPerBlockExpected,
        sampleRate);
    reverbSrc.prepareToPlay( // reverb control
        samplesPerBlockExpected,
        sampleRate);

}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    reverbSrc.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSrc.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    else
    {
        DBG("Something went wrong loading the file");
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else
    {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 10");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

double DJAudioPlayer::getPositionRelative() 
{
    if (transportSource.getLengthInSeconds() > 0) 
    {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}

void DJAudioPlayer::setRoomSizeLevel(float roomSizeValue)
{
    if (roomSizeValue < 0 || roomSizeValue > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSizeLevel roomSizeValue should be between 0 and 1");
    }
    else
    {
        reverbParams.roomSize = roomSizeValue;
        reverbSrc.setParameters(reverbParams);
    }
}

void DJAudioPlayer::setDampLevel(float dampAmt)
{
    if (dampAmt < 0 || dampAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDampLevel dampAmt should be between 0 and 1");
    }
    else
    {
        reverbParams.damping = dampAmt;
        reverbSrc.setParameters(reverbParams);
    }
}

void DJAudioPlayer::setDryLevel(float dryValue)
{
    if (dryValue < 0 || dryValue > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel dryValue should be between 0 and 1");
    }
    else
    {
        reverbParams.dryLevel = dryValue;
        reverbSrc.setParameters(reverbParams);
    }
}

void DJAudioPlayer::setWetLevel(float wetValue)
{
    if (wetValue < 0 || wetValue > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel wetValue should be between 0 and 1");
    }
    else
    {
        reverbParams.wetLevel = wetValue;
        reverbSrc.setParameters(reverbParams);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::pause()
{
    transportSource.stop();
}

void DJAudioPlayer::stop()
{
    transportSource.setPosition(0);
    transportSource.stop();
}

void DJAudioPlayer::loop()
{
    transportSource.setPosition(0);
    transportSource.start();
}

double DJAudioPlayer::getDurationInSeconds() 
{
    return transportSource.getLengthInSeconds();
}
