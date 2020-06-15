//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_SONG_H
#define WET2_SONG_H


#include "Tree.h"
#include "RankTree.h"
#include "ThreeParamKey.h"

class Song {
private:
    int songID;
    int artistID;
    int numberOfPlays;

public:
    Song(int songID, int artistID, int numberOfPlays = 0);

    int getSongId() const;

    void setSongId(int nSongID);

    int getArtistId() const;

    void setArtistId(int nArtistID);

    int getNumberOfPlays() const;

    void setNumberOfPlays(int nNumberOfPlays);

    ~Song() = default;
};


#endif //WET2_SONG_H
