//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_MUSICMANAGER_H
#define WET2_MUSICMANAGER_H


#include "library2.h"
#include "HashTable.h"
#include "Artist.h"
#include "RankTree.h"
#include "ThreeParamKey.h"
#include "SongContainer.h"

class MusicManager {
private:
    HashTable<Artist> artistHashTable;
    RankTree<ThreeParamKey, SongContainer> songRankTree;
    int numberOfSongs;
    int numberOfArtists;
public:
    MusicManager();

    StatusType AddArtist(int artistID);

    StatusType RemoveArtist(int artistID);

    StatusType AddSong(int artistID, int songID);

    StatusType RemoveSong(int artistID, int songID);

    StatusType AddToSongCount(int artistID, int songID, int count);

    StatusType GetArtistBestSong(int artistID, int *songID);

    StatusType GetRecommendedSongInPlace(int rank, int *artistID, int *songID);

    ~MusicManager();
};


#endif //WET2_MUSICMANAGER_H
