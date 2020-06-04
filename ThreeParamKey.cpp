//
// Created by Mor on 03/06/2020.
//

#include "ThreeParamKey.h"

ThreeParamKey::ThreeParamKey() : numberOfPlays(0), songID(1), artistID(1) {

}

ThreeParamKey::ThreeParamKey(int numberOfPlays, int songId, int artistId) : numberOfPlays(numberOfPlays), songID(songId), artistID(artistId) {

}

bool ThreeParamKey::operator==(const ThreeParamKey &other) {
    return (numberOfPlays == other.numberOfPlays) && (songID == other.songID) && (artistID == other.artistID);
}

bool ThreeParamKey::operator!=(const ThreeParamKey &other) {
    return !(operator==(other));
}

bool ThreeParamKey::operator>(const ThreeParamKey &other) {
    if (numberOfPlays == other.numberOfPlays) {
        if (artistID == other.artistID) {
            return (songID < other.songID);
        } else {
            return (artistID < other.artistID);
        }
    } else {
        return (numberOfPlays > other.numberOfPlays);
    }
}

bool ThreeParamKey::operator>=(const ThreeParamKey &other) {
    return (operator>(other) || operator==(other));
}

bool ThreeParamKey::operator<(const ThreeParamKey &other) {
    if (numberOfPlays == other.numberOfPlays) {
        if (artistID == other.artistID) {
            return (songID > other.songID);
        } else {
            return (artistID > other.artistID);
        }
    } else {
        return (numberOfPlays < other.numberOfPlays);
    }
}

bool ThreeParamKey::operator<=(const ThreeParamKey &other) {
    return (operator<(other) || operator==(other));
}

int ThreeParamKey::getNumberOfPlays() const {
    return numberOfPlays;
}

void ThreeParamKey::setNumberOfPlays(int nNumPlays) {
    ThreeParamKey::numberOfPlays = nNumPlays;
}

int ThreeParamKey::getSongId() const {
    return songID;
}

void ThreeParamKey::setSongId(int nSongID) {
    ThreeParamKey::songID = nSongID;
}

int ThreeParamKey::getArtistId() const {
    return artistID;
}

void ThreeParamKey::setArtistId(int nArtistID) {
    ThreeParamKey::artistID = nArtistID;
}