/*
  ==============================================================================

    WaveformDisplay.h
    Created: 1 Aug 2022 1:35:14pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                         public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster *source) override;
    
    void loadURL(URL audioURL);

    /** set the relative position of playhead */
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
