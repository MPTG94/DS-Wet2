//
// Created by Mor on 03/06/2020.
//

#include "ThreeParamKey.h"

ThreeParamKey::ThreeParamKey() : numberOfPlays(0), songId(1), artistId(1) {

}

ThreeParamKey::ThreeParamKey(int numberOfPlays, int songId, int artistId) : numberOfPlays(numberOfPlays), songId(songId), artistId(artistId) {

}

bool ThreeParamKey::operator==(const ThreeParamKey &other) {
    return (numberOfPlays == other.numberOfPlays) && (songId == other.songId) && (artistId == other.artistId);
}

bool ThreeParamKey::operator!=(const ThreeParamKey &other) {
    return !(operator==(other));
}

bool ThreeParamKey::operator>(const ThreeParamKey &other) {
    if (numberOfPlays == other.numberOfPlays) {
        if (artistId == other.artistId) {
            return (songId < other.songId);
        } else {
            return (artistId < other.artistId);
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
        if (artistId == other.artistId) {
            return (songId > other.songId);
        } else {
            return (artistId > other.artistId);
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
    return songId;
}

void ThreeParamKey::setSongId(int nSongId) {
    ThreeParamKey::songId = nSongId;
}

int ThreeParamKey::getArtistId() const {
    return artistId;
}

void ThreeParamKey::setArtistId(int nArtistId) {
    ThreeParamKey::artistId = nArtistId;
}