/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 2 Aug 2022 11:46:28am
    Author:  Mohan Krishna

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* deck1,
                                     DeckGUI* deck2, 
                                     DJAudioPlayer* playerToProcessTrackData) 
                                     : deck1(deck1),
                                       deck2(deck2),
                                       playerToProcessTrackData(playerToProcessTrackData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // make the buttons and components visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadLibraryButton);
    addAndMakeVisible(loadToDeck1Button);
    addAndMakeVisible(loadToDeck2Button);
    addAndMakeVisible(searchBar);

    // playlist table header columns
    tableComponent.getHeader().addColumn("Track title", 1, 210);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Audio Format", 3, 100);
    tableComponent.getHeader().addColumn("Delete", 4, 100);

    // add listener
    loadLibraryButton.addListener(this);
    loadToDeck1Button.addListener(this);
    loadToDeck2Button.addListener(this);
    searchBar.addListener(this);

    // show placeholder text for search bar
    searchBar.setTextToShowWhenEmpty("Search Playlist Tracks", Colours::yellow);

    // run a search function on text entered in search bar
    searchBar.onReturnKey = [this] { searchTrackPlaylist(searchBar.getText()); };

    loadLibraryButton.setColour(ComboBox::outlineColourId, Colours::aqua);
    loadToDeck1Button.setColour(ComboBox::outlineColourId, Colours::aqua);
    loadToDeck2Button.setColour(ComboBox::outlineColourId, Colours::aqua);

    // search bar colour styling
    searchBar.setColour(TextEditor::ColourIds::backgroundColourId, Colours::black);
    searchBar.setColour(TextEditor::outlineColourId, Colours::darkgrey);        

    tableComponent.setModel(this);

    // load the tracks saved in the playlist
    loadTrackPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    // save playlist tracks when PlaylistComponent is destroyed
    savePlaylist();

    // set tabelComponent model to null
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (Graphics& g)
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

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double x_pos = 10;

    double rowH = (getHeight() - 22) / 16;
    double componentWidth = (getWidth() - 22);

    // search bar, playlist table, load into library and into decks options
    searchBar.setBounds(x_pos, x_pos, componentWidth, rowH);
    tableComponent.setBounds(x_pos, rowH + x_pos + 10, componentWidth, rowH * 12);
    loadLibraryButton.setBounds(x_pos, rowH * 15 + x_pos, componentWidth, rowH);
    loadToDeck1Button.setBounds(x_pos, rowH * 14, componentWidth / 2 - 10, rowH);
    loadToDeck2Button.setBounds(x_pos + 5 + componentWidth / 2 - 5, rowH * 14, componentWidth/ 2, rowH);

    tableComponent.setColour(ListBox::backgroundColourId, Colours::black);

}

int PlaylistComponent::getNumRows() 
{
    return tracksData.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected) 
    {
        g.fillAll(Colours::lightblue);
    }
    else
    {
        g.fillAll(Colours::black);
        g.setColour(Colours::white);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        // track title column
        if (columnId == 1)
        {
            g.drawText(tracksData[rowNumber].trackTitle,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        // track duration column
        if (columnId == 2)
        {
            g.drawText(tracksData[rowNumber].duration,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        // track file format column
        if (columnId == 3)
        {
            g.drawText(tracksData[rowNumber].format,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    // Delete button option to delete a track from playlist
    // create a pointer to the track id for each delete button
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Delete");

            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::black);
            btn->setColour(ComboBox::outlineColourId, Colours::red);

            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadLibraryButton)
    {
        loadIntoPlaylist();
        tableComponent.updateContent(); // update table contents
    }
    else if (button == &loadToDeck1Button)
    {
        loadToDeck(deck1);
    }
    else if (button == &loadToDeck2Button)
    {
        loadToDeck(deck2);
    }
    else
    {
        int trackId = std::stoi(button->getComponentID().toStdString());

        // Delete confirmation alert to delete selected track from playlist
        // If the Delete option is selected, then track is deleted

        if (AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon,
            "Are you sure?",
            "The selected track will be deleted from the music library",
            "Delete",
            "Cancel",
            nullptr) == true)
        {
            deleteTrackFromMusicLibrary(trackId);

            // Alert to confirm deletion of track from playlist
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                " ",
                "Track has been deleted from the music library",
                "Okay",
                nullptr
            );
        }
        tableComponent.updateContent(); // update table contents
    }
}

void PlaylistComponent::loadIntoPlaylist()
{
    FileChooser fChooser{ "Select a file(s) to be added into the music library..." };
    
    if (fChooser.browseForMultipleFilesToOpen())
    {
        for (const File& track : fChooser.getResults())
        {
            String titleOfTrack{ track.getFileNameWithoutExtension() };

            // check if track is already in playlist
            if (!isTrackInPlaylist(titleOfTrack))
            {
                TrackData newTrack{ track };

                URL trackURL{ track };

                // getTrackDuration will obtain the track length in seconds and converts it into minutes
                newTrack.duration = getTrackDuration(trackURL);

                // add track to the track list
                tracksData.push_back(newTrack);
            } 
            // prevent adding duplicates of tracks in playlist
            else
            {   
                // Alert to tell the user that the track is already in the playlist
                AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "", titleOfTrack +
                                            " already exists in the music playlist",
                                            "Okay",
                                            nullptr
                                            );
            }
        }
    }
}

void PlaylistComponent::loadToDeck(DeckGUI* deck)
{
    // A track has to be slected from the playlist in order to be addded to a chosen deck

    // obtain the track selected by user from the playlist
    int selectedTrack{ tableComponent.getSelectedRow() };

    if (selectedTrack != -1)
    {
        // load track into deck
        deck->loadTrack(tracksData[selectedTrack].trackURL);
    }
    else
    {   
        // alert the user to select a track from the playlist before proceeding
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
            "",
            "Please select a track from the playlist",
            "OK",
            nullptr
        );

    }
}

bool PlaylistComponent::isTrackInPlaylist(String title) 
{
    return (std::find(tracksData.begin(), tracksData.end(), title) != tracksData.end());
}

void PlaylistComponent::searchTrackPlaylist(String text)
{
    // highlight the track row that matches the search
    // else, deselect all other rows

    if (text != "") 
    {
        int FoundTrack = searchInputText(text);

        tableComponent.selectRow(FoundTrack);
    }
    else
    {
        tableComponent.deselectAllRows();
    }
}

int PlaylistComponent::searchInputText(String search)
{
    // reference https://www.geeksforgeeks.org/stdfind_if-stdfind_if_not-in-c/

    auto list = std::find_if
    (
        tracksData.begin(),
        tracksData.end(),

        [&search](const TrackData& data)
        {
            return data.trackTitle.contains(search);
        }
    );

    int j = -1;

    if (list != tracksData.end())
    {
        j = std::distance(tracksData.begin(), list);
    }

    return j;
}

void PlaylistComponent::deleteTrackFromMusicLibrary(int trackId)
{
    tracksData.erase(tracksData.begin() + trackId);

}

void PlaylistComponent::savePlaylist()
{
    // uses 'algorithm' module included in header,
    // this creates a csv file to store the playlist tracks

    std::ofstream  myPlaylistCSV("trackplaylist.csv");

    // store current playlist tracks into the csv file
    for (TrackData& p : tracksData)
    {
        myPlaylistCSV << p.track.getFullPathName() << "," << p.duration << "\n";
    }
}

void PlaylistComponent::loadTrackPlaylist()
{
    // source: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c

    std::ifstream myPlaylistCSV("trackplaylist.csv");

    std::string pathName; // track file path name
    std::string duration; // track duration

    if (myPlaylistCSV.is_open())
    {
        while (std::getline(myPlaylistCSV, pathName, ',')) 
        {
            File track{ pathName };
            TrackData newTrack{ track };

            std::getline(myPlaylistCSV, duration);

            newTrack.duration = duration;
            tracksData.push_back(newTrack);
        }
    }
    // close the file (.csv file) after parsing and extracting data
    myPlaylistCSV.close();
}

// additional funtions to help find and convert the track duration

String PlaylistComponent::getTrackDuration(URL trackURL)
{
    playerToProcessTrackData->loadURL(trackURL);
    double secs{ playerToProcessTrackData->getDurationInSeconds() };

    // Call the function to convert and get final track duration in minutes
    String mins{ getTrackDurationInMinutes(secs) };
    return mins;
}

String PlaylistComponent::getTrackDurationInMinutes(double secs)
{
    int secsApprox{ int(std::round(secs)) };

    // get time duration in minutes and seconds using secsApprox

    String TimeInMins{ std::to_string(secsApprox / 60) };

    String TimeInSecs{ std::to_string(secsApprox % 60) };

    // handle one digit minutes and seconds
    // eg: 2 min -> 02 min and 4 sec -> 04 sec

    if (TimeInMins.length() < 2)
    {
        TimeInMins = TimeInMins.paddedLeft('0', 2);
    }
    if (TimeInSecs.length() < 2)
    {
        TimeInSecs = TimeInSecs.paddedLeft('0', 2);
    }

    // Track duration displayed in minutes and seconds
    return String{ TimeInMins + ":" + TimeInSecs };
}
