//
// Created by Mor on 03/06/2020.
//

#include "MusicManager.h"

StatusType MusicManager::AddArtist(int artistID) {

    Artist *nArtist = new Artist(artistID);
    if(artistHashTable.Insert(artistID, nArtist) == FAILURE) {
        // The artist already exist
        delete  nArtist;
        return FAILURE;
    }
    return SUCCESS;
}

StatusType MusicManager::RemoveArtist(int artistID) {

    Artist *rArtist = artistHashTable.FindNode(artistID)->getData();
    if(rArtist== nullptr || rArtist->getNumberOfSongs()!=0) {
        // The artist doesn't exist or
        // The artist exist but can't be removed because he has songs
        return FAILURE;
    }

    if(artistHashTable.Remove(artistID) != SUCCESS){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType MusicManager::AddSong(int artistID, int songID) {
    return INVALID_INPUT;
}

StatusType MusicManager::RemoveSong(int artistID, int songID) {
    return INVALID_INPUT;
}

StatusType MusicManager::AddToSongCount(int artistID, int songID, int count) {
    return INVALID_INPUT;
}

StatusType MusicManager::GetArtistBestSong(int artistID, int *songID) {
    return INVALID_INPUT;
}

StatusType MusicManager::GetRecommendedSongInPlace(int rank, int *artistID, int *songID) {
    return INVALID_INPUT;
}

MusicManager::~MusicManager() {

}
