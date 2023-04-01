/*
  ==============================================================================

    DeckGUI.cpp
    Created: 18 Jul 2022 1:19:02pm
    Author:  Mohan Krishna

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse) : 
                 player(_player),
                 waveformDisplay(formatManagerToUse, cacheToUse)
{
    // make buttons and sliders visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(dampLevelSlider);
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(wetLevelSlider);

    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(volSliderLabel);

    addAndMakeVisible(roomSizeLabel);
    addAndMakeVisible(dampLevelLabel);
    addAndMakeVisible(dryLevelLabel);
    addAndMakeVisible(wetLevelLabel);
    

    addAndMakeVisible(waveformDisplay);


    // add a listener for the gui components (buttons and sliders)
    playButton.addListener(this);
    pauseButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    roomSizeSlider.addListener(this);
    dampLevelSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);

    // set range for sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);
    roomSizeSlider.setRange(0.0, 1.0);
    dampLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setRange(0.0, 1.0);
    dryLevelSlider.setRange(0.0, 1.0);

    // position slider settings
    posSlider.setTextBoxStyle(Slider::NoTextBox, true, 60, 15);
    posSliderLabel.setText("Playback", NotificationType::dontSendNotification);
    posSliderLabel.attachToComponent(&posSlider, true);

    // volume slider settings
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    volSlider.setSliderStyle(Slider::LinearVertical);
    volSliderLabel.setText("Volume", NotificationType::dontSendNotification);
    volSliderLabel.attachToComponent(&volSlider, false);

    // audio size level slider settings
    roomSizeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    roomSizeSlider.setSliderStyle(Slider::LinearVertical);
    roomSizeLabel.setText("Room Size", NotificationType::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, true);

    // audio damp level slider settings
    dampLevelSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    dampLevelSlider.setSliderStyle(Slider::LinearVertical);
    dampLevelLabel.setText("Damp Level", NotificationType::dontSendNotification);
    dampLevelLabel.attachToComponent(&dampLevelSlider, true);

    // audio dryness level slider settings
    dryLevelSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    dryLevelSlider.setSliderStyle(Slider::Rotary);
    dryLevelLabel.setText("Dry Level", NotificationType::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevelSlider, true);

    // audio wetness level slider settings
    wetLevelSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    wetLevelSlider.setSliderStyle(Slider::Rotary);
    wetLevelLabel.setText("Wet Level", NotificationType::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevelSlider, true);

    // speed slider settings
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSliderLabel.setText("Speed", NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, true);

    startTimer(200);

    // colour styling of play, pause, stop, and load button
    playButton.setColour(ComboBox::outlineColourId, Colours::aqua);
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);

    pauseButton.setColour(ComboBox::outlineColourId, Colours::aqua);
    pauseButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);

    stopButton.setColour(ComboBox::outlineColourId, Colours::violet);
    stopButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);

    loadButton.setColour(ComboBox::outlineColourId, Colours::green);
    loadButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.fillAll(Colours::black);

    g.setColour (Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{

    double pos = 10;
    double rowH = getHeight() / 8;
    double sliderHeight = rowH + 23;
    double width = (getWidth() / 8);
    double btnWidth = (getWidth() - 90) / 5;
    double sliderPos = width / 2;
    double verticalSliderWidth = getWidth() / 7;
    double rotatorySliderWidth = getWidth() / 6;

    playButton.setBounds(pos, pos, btnWidth, rowH);
    pauseButton.setBounds(getWidth()/5, pos, btnWidth, rowH);
    stopButton.setBounds(getWidth() / 5 + btnWidth + 8, pos, btnWidth, rowH);

    loopButton.setBounds(getWidth() / 5 + btnWidth * 3, pos, btnWidth / 3, rowH);

    volSlider.setBounds(sliderPos/2, sliderHeight, verticalSliderWidth, rowH * 2.3);
    volSliderLabel.setCentreRelative(0.14f, 0.51f);

    roomSizeSlider.setBounds(verticalSliderWidth + pos * 2, sliderHeight, verticalSliderWidth, rowH * 2.3);
    roomSizeLabel.setCentreRelative(0.24f, 0.51f);

    dampLevelSlider.setBounds(verticalSliderWidth * 2.1, sliderHeight, verticalSliderWidth, rowH * 2.3);
    dampLevelLabel.setCentreRelative(0.37f, 0.51f);

    dryLevelSlider.setBounds(getWidth() / 2, sliderHeight, rotatorySliderWidth, rowH * 2.3);
    dryLevelLabel.setCentreRelative(0.58f, 0.51f);

    wetLevelSlider.setBounds(getWidth() / 2.05 + rotatorySliderWidth, sliderHeight, rotatorySliderWidth, rowH * 2.3);
    wetLevelLabel.setCentreRelative(0.735f, 0.51f);

    speedSlider.setBounds(getWidth()/1.24, sliderHeight, rotatorySliderWidth, rowH * 2.3);
    speedSliderLabel.setCentreRelative(0.89f, 0.51f);

    posSlider.setBounds(getWidth() / 7.5, rowH * 4.5, getWidth()/1.2, rowH);
    posSliderLabel.setCentreRelative(0.08f, 0.612f);

    waveformDisplay.setBounds(pos, rowH * 5.5, getWidth() - 20, rowH * 2.4);
    loadButton.setBounds(getWidth()/1.2 - pos, pos, getWidth() / 6, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();
    }
    if (button == &pauseButton)
    {
        DBG("Pause button was clicked");
        player->pause();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("Load button was clicked");

        auto fileChooserFlags =
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            auto chosenFile = chooser.getResult();
            player->loadURL(URL{ chosenFile });
            waveformDisplay.loadURL(URL{ chosenFile });
        });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &roomSizeSlider)
    {
        player->setRoomSizeLevel(slider->getValue());
    }

    if (slider == &dampLevelSlider)
    {
        player->setDampLevel(slider->getValue());
    }

    if (slider == &dryLevelSlider)
    {
        player->setDryLevel(slider->getValue());
    }

    if (slider == &wetLevelSlider)
    {
        player->setWetLevel(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]} });
        waveformDisplay.loadURL(URL{ File{files[0] } });
    }
}

void DeckGUI::timerCallback()
{
    std::string checkedValue = std::to_string(loopButton.getToggleState());

    // check if loop button has been checked
    if (checkedValue == "1") {
        if (player->getPositionRelative() >= 1) { // check if player is the end of audio file

            // loops the audio file
            player->loop();
        }
    }
    // check if loop button has not been checked
    if (checkedValue == "0") {
        if (player->getPositionRelative() >= 1) { // check if player is the end of audio file

            // stops the audio file at the start position of the file
            player->stop();
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative()); 
}

void DeckGUI::loadTrack(URL trackURL)
{
    player->loadURL(trackURL);
    waveformDisplay.loadURL(trackURL);
}
