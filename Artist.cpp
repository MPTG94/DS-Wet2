//
// Created by Mor on 03/06/2020.
//

#include "Artist.h"

Artist::Artist(int artistID) : artistID(artistID), numberOfSongs(0), bestSong(nullptr) {
    songsByIdTree = Tree<Song>();
    songsByPlaysTree = RankTree<TwoParamKey, Song>();
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

RankTree<TwoParamKey, Song> &Artist::getSongsByPlaysTree() {
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

void Artist::compareNumberOfSongs() {
    if (!songsByPlaysTree.GetRoot()) {
        return;
    }
    if (numberOfSongs != songsByPlaysTree.GetRoot()->getRank()) {
        std::cout << "ERROR IN NUMBER OF SONGS, artistID: " << artistID << " Number of songs: " << numberOfSongs << " Rank of root: "
                  << songsByPlaysTree.GetRoot()->getRank() << std::endl;
    }
}

