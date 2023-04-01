/*
  ==============================================================================

    DeckGUI.h
    Created: 18 Jul 2022 1:19:02pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;

//==============================================================================
/*
*/
class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

    void loadTrack(URL trackURL); // laod track in deck

private:

    TextButton playButton{ "PLAY" };
    TextButton pauseButton{ "PAUSE" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    ToggleButton loopButton{ "LOOP" };

    Slider volSlider;
    Slider posSlider;
    Slider speedSlider;
    Slider roomSizeSlider;
    Slider dampLevelSlider;
    Slider dryLevelSlider;
    Slider wetLevelSlider;

    Label posSliderLabel;
    Label volSliderLabel;
    Label speedSliderLabel;
    Label roomSizeLabel;
    Label dampLevelLabel;
    Label dryLevelLabel;
    Label wetLevelLabel;

    FileChooser fChooser{ "Select a file..." };

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
