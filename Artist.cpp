//
// Created by Mor on 03/06/2020.
//

#include "Artist.h"

Artist::Artist(int artistID) : artistID(artistID), numberOfSongs(0), bestSong(nullptr) {
    songsByIdTree = Tree<Song>();
    songsByPlaysTree = Tree<Song>();
}

int Artist::getArtistId() const {
    return artistID;
}

int Artist::getNumberOfSongs() const {
    return numberOfSongs;
}

Song *Artist::getBestSong() const {
    return bestSong;
}

Tree<Song> &Artist::getSongsByIdTree() {
    return songsByIdTree;
}

Tree<Song> &Artist::getSongsByPlaysTree() {
    return songsByPlaysTree;
}

void Artist::setArtistId(int artistId) {
    artistID = artistId;
}

void Artist::setNumberOfSongs(int NumberOfSongs) {
    Artist::numberOfSongs = NumberOfSongs;
}

void Artist::setBestSong(Song *BestSong) {
    Artist::bestSong = BestSong;
}

