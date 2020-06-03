//
// Created by Mor on 03/06/2020.
//

#include "Artist.h"

Artist::Artist(int artistID) : artistID(artistID), numberOfSongs(0), bestSong(nullptr){

    songsByIdTree = new Tree<Song>(0);

    songsByPlaysTree = new Tree<Song>(0);
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

Tree<Song> *Artist::getSongsByIdTree() const {
    return songsByIdTree;
}

Tree<Song> *Artist::getSongsByPlaysTree() const {
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

void Artist::setSongsByIdTree(Tree<Song> *SongsByIdTree) {
    Artist::songsByIdTree = SongsByIdTree;
}

void Artist::setSongsByPlaysTree(Tree<Song> *SongsByPlaysTree) {
    Artist::songsByPlaysTree = SongsByPlaysTree;
}

Artist::~Artist() {

}

