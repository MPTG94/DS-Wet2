//
// Created by Mor on 03/06/2020.
//

#include "Song.h"

Song::Song(int songID, int artistID, int numberOfPlays) : songID(songID), artistID(artistID),
                                                          numberOfPlays(numberOfPlays) {

}

int Song::getSongId() const {
    return songID;
}

void Song::setSongId(int nSongID) {
    songID = nSongID;
}

int Song::getArtistId() const {
    return artistID;
}

void Song::setArtistId(int nArtistID) {
    artistID = nArtistID;
}

int Song::getNumberOfPlays() const {
    return numberOfPlays;
}

void Song::setNumberOfPlays(int nNumberOfPlays) {
    Song::numberOfPlays = nNumberOfPlays;
}