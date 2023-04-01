/*
  ==============================================================================

    PlaylistComponent.h
    Created: 2 Aug 2022 11:46:28am
    Author:  Mohan Krishna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "TrackData.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include <fstream>
#include <algorithm>

using namespace juce;

//==============================================================================
/*
*/
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener
{
public:
    PlaylistComponent (DeckGUI* deck1, 
                       DeckGUI* deck2, 
                       DJAudioPlayer* playerToProcessTrackData
                       );

    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override; // returns the total number of table rows

    void paintRowBackground(Graphics&, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override; // paints the row background

    void paintCell(Graphics&, 
                   int rowNumber, 
                   int columnId, 
                   int width, 
                   int height, 
                   bool rowIsSelected) override; // paints the table cells

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

private:
    AudioFormatManager formatManager;

    // tracks stored and loaded into playlist displayed here
    TableListBox tableComponent;

    // search bar to search tracks in playlist
    TextEditor searchBar;

    // load tracks into music library from system
    TextButton loadLibraryButton{ "Load track into library" };

    // load track from playlist to decks
    TextButton loadToDeck1Button{ "Load In DECK 1" };
    TextButton loadToDeck2Button{ "Load In DECK 2" };

    // import TrackData class from TrackData.h file, to store track data in a vector
    std::vector<TrackData> tracksData;

    // load track into playlist
    void loadIntoPlaylist();

    // load track into decks
    void loadToDeck(DeckGUI* deckGUI);

    // check if track is in playlist
    bool isTrackInPlaylist(String TrackTitle);

    // search playlist for track
    void searchTrackPlaylist(String text);

    // search track based on input text entered in search bar
    int searchInputText(String search);

    // delete selected track from playlist
    void deleteTrackFromMusicLibrary(int trackId);

    // save playlist after closing program
    void savePlaylist();

    // load track playlist
    void loadTrackPlaylist();

    // Additional functions to find the track duration
    String getTrackDuration(URL trackURL);

    String getTrackDurationInMinutes(double secs);

    DeckGUI* deck1;
    DeckGUI* deck2;

    DJAudioPlayer* playerToProcessTrackData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
