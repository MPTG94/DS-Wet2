//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_ARTIST_H
#define WET2_ARTIST_H


#include "Tree.h"
#include "Song.h"

class Artist {
private:
    int artistID;
    int numberOfSongs;
    Song* bestSong;
    Tree<Song> songsByIdTree;
    Tree<Song> songsByPlaysTree;
};


#endif //WET2_ARTIST_H
