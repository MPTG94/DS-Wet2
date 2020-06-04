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

public:
    explicit Artist(int artistID);

    int getArtistId() const;

    int getNumberOfSongs() const;

    Song *getBestSong() const;

    Tree<Song> &getSongsByIdTree();

    Tree<Song> &getSongsByPlaysTree();

    void setArtistId(int artistId);

    void setNumberOfSongs(int numberOfSongs);

    void setBestSong(Song *bestSong);

    ~Artist() = default;


};


#endif //WET2_ARTIST_H
