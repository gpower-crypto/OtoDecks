/*
  ==============================================================================

    TrackData.h
    Created: 14 Aug 2022 1:53:11pm
    Author:  Mohan Krishna

    This component is meant to parse track data and use in the playlistComponent.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

using namespace juce;

class TrackData {

public:

    TrackData(File playlistTrack);
    File track;
    String format;
    URL trackURL;
    String trackTitle;
    String duration;

    bool operator == (const String& trackData) const;
};