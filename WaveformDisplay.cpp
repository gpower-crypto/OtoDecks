/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 1 Aug 2022 1:35:14pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0.0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::black);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::darkorange);
    g.setFont (20.0f);
    if (fileLoaded) {
        audioThumb.drawChannel(g, 
            getLocalBounds(), 
            0, 
            audioThumb.getTotalLength(),
            0,
            1.0f
          );
        g.setColour(Colours::lightyellow);
        
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());

    }
    else 
    {
        g.drawText("File not loaded yet....", getLocalBounds(),
        Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded) {
        DBG("wfd: loaded!");
        repaint();
    }
    else {
        DBG("wfd: not loaded");
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) 
{
    DBG("wfd: change received!");
    repaint();
}


void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position) {
        position = pos;
        repaint();
    }
}
