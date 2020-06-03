//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_SONG_H
#define WET2_SONG_H


#include "Tree.h"
#include "RankTree.h"
#include "ThreeParamKey.h"
#include "SongContainer.h"

class Song {
private:
    int songID;
    int artistID;
    int numberOfPlays;
    RankTreeNode<ThreeParamKey, SongContainer>* ptrToRankedSong;
    Song* ptrToSongInTree;
};


#endif //WET2_SONG_H
