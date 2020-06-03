//
// Created by Mor on 03/06/2020.
//


#include <new>
#include "library2.h"
#include "MusicManager.h"

using std::bad_alloc;

void *Init() {
    try {
        MusicManager *DS = new MusicManager();
        if (!DS) {
            return nullptr;
        }
        return (void *) DS;
    } catch (std::bad_alloc &e) {
        return nullptr;
    }
}

StatusType AddArtist(void *DS, int artistID) {
    if (!DS || artistID <= 0) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->AddArtist(artistID);
}

StatusType RemoveArtist(void *DS, int artistID) {
    if (!DS || artistID <= 0) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->RemoveArtist(artistID);
}

StatusType AddSong(void *DS, int artistID, int songID) {
    if (!DS || artistID <= 0 || songID <= 0) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->AddSong(artistID, songID);
}

StatusType RemoveSong(void *DS, int artistID, int songID) {
    if (!DS || artistID <= 0 || songID <= 0) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->RemoveSong(artistID, songID);
}

StatusType AddToSongCount(void *DS, int artistID, int songID, int count) {
    if (!DS || artistID <= 0 || songID <= 0 || count <= 0) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->AddToSongCount(artistID, songID, count);
}

StatusType GetArtistBestSong(void *DS, int artistID, int *songID) {
    if (!DS || artistID <= 0 || !songID) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->GetArtistBestSong(artistID, songID);
}

StatusType GetRecommendedSongInPlace(void *DS, int rank, int *artistID, int *songID) {
    if (!DS || rank <= 0 || !artistID || !songID) {
        return INVALID_INPUT;
    }
    MusicManager *nDS = static_cast<MusicManager *>(DS);
    return nDS->GetRecommendedSongInPlace(rank, artistID, songID);
}

void Quit(void **DS) {
    MusicManager *nDS = static_cast<MusicManager *>(*DS);
    delete nDS;
    nDS = nullptr;
    *DS = nullptr;
}