//
// Created by Mor on 04/06/2020.
//

#include "TwoParamKey.h"

TwoParamKey::TwoParamKey() : numberOfPlays(0), songID(1) {

}

TwoParamKey::TwoParamKey(int numberOfPlays, int songID) : numberOfPlays(numberOfPlays), songID(songID) {

}

bool TwoParamKey::operator==(const TwoParamKey &other) {
    return (numberOfPlays == other.numberOfPlays) && (songID == other.songID);
}

bool TwoParamKey::operator!=(const TwoParamKey &other) {
    return !(operator==(other));
}

bool TwoParamKey::operator>(const TwoParamKey &other) {
    if (numberOfPlays == other.numberOfPlays) {
        return (songID < other.songID);
    } else {
        return (numberOfPlays > other.numberOfPlays);
    }
}

bool TwoParamKey::operator>=(const TwoParamKey &other) {
    return (operator>(other) || operator==(other));
}

bool TwoParamKey::operator<(const TwoParamKey &other) {
    if (numberOfPlays == other.numberOfPlays) {
        return (songID > other.songID);
    } else {
        return (numberOfPlays < other.numberOfPlays);
    }
}

bool TwoParamKey::operator<=(const TwoParamKey &other) {
    return (operator<(other) || operator==(other));
}

int TwoParamKey::getNumberOfPlays() const {
    return numberOfPlays;
}

void TwoParamKey::setNumberOfPlays(int nNumPlays) {
    TwoParamKey::numberOfPlays = nNumPlays;
}

int TwoParamKey::getSongId() const {
    return songID;
}

void TwoParamKey::setSongId(int nSongID) {
    TwoParamKey::songID = nSongID;
}