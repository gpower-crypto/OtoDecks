/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"

#include "DeckGUI.h"

#include "PlaylistComponent.h"

using namespace juce;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};

    MixerAudioSource mixerSource;

    DJAudioPlayer playerToProcessTrackData{ formatManager };

    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &playerToProcessTrackData };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
