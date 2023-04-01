/*
  ==============================================================================

    TrackData.cpp
    Created: 14 Aug 2022 1:53:11pm
    Author:  Mohan Krishna

    This component is meant to parse track data and use in the playlistComponent.

  ==============================================================================
*/

#include "TrackData.h"
#include <vector>
#include <string>
#include <filesystem>

TrackData::TrackData(File playlistTrack) : track(playlistTrack),
                                           trackTitle(playlistTrack.getFileNameWithoutExtension()),
                                           trackURL(URL {playlistTrack}),
                                           format(playlistTrack.getFileExtension())

{
    DBG("Track - " << trackTitle << " added to the playlist.");
}

bool TrackData::operator == (const String& data) const 
{
    return trackTitle == data;
}