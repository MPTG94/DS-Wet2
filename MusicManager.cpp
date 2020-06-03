//
// Created by Mor on 03/06/2020.
//

#include "MusicManager.h"

StatusType MusicManager::AddArtist(int artistID) {
    return INVALID_INPUT;
}

StatusType MusicManager::RemoveArtist(int artistID) {
    return INVALID_INPUT;
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
