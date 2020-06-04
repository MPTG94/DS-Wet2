//
// Created by Mor on 03/06/2020.
//

#include "MusicManager.h"

MusicManager::MusicManager() : artistHashTable(HashTable<Artist>()), songRankTree(RankTree<ThreeParamKey, int>()), numberOfSongs(0),
                               numberOfArtists(0) {

}

StatusType MusicManager::AddArtist(int artistID) {

    Artist *nArtist = new Artist(artistID);
    if (artistHashTable.Insert(artistID, nArtist) == FAILURE) {
        // The artist already exist
        delete nArtist;
        return FAILURE;
    }
    numberOfArtists++;
    return SUCCESS;
}

StatusType MusicManager::RemoveArtist(int artistID) {

    Artist *rArtist = artistHashTable.FindNode(artistID)->getData();
    if (rArtist == nullptr || rArtist->getNumberOfSongs() != 0) {
        // The artist doesn't exist or
        // The artist exist but can't be removed because he has songs
        return FAILURE;
    }

    if (artistHashTable.Remove(artistID) != SUCCESS) {
        return FAILURE;
    }
    numberOfArtists--;
    return SUCCESS;
}

StatusType MusicManager::AddSong(int artistID, int songID) {
    if (numberOfArtists <= 0 || artistHashTable.Find(artistID) != SUCCESS) {
        return FAILURE;
    }

    Artist *artist = artistHashTable.FindNode(artistID)->getData();
    if (artist->getSongsByIdTree().Find(songID)) {
        return FAILURE;
    }

    StatusType additionResult = AddSongToArtist(artistID, songID, artist);
    if (additionResult == SUCCESS) {
        numberOfSongs++;
        return SUCCESS;
    } else {
        return ALLOCATION_ERROR;
    }
}

StatusType MusicManager::RemoveSong(int artistID, int songID) {
    if (numberOfArtists <= 0 || artistHashTable.Find(artistID) != SUCCESS) {
        return FAILURE;
    }

    Artist *artist = artistHashTable.FindNode(artistID)->getData();
    TreeNode<Song> *songNode = artist->getSongsByIdTree().Find(songID);
    if (!songNode) {
        // The song doesn't exist
        return FAILURE;
    }

    Song *song = songNode->getData();
    ThreeParamKey songKey = ThreeParamKey(song->getNumberOfPlays(), songID, artistID);
    songRankTree.Remove(songKey);

    Song *bestSong = artist->getBestSong();
    TwoParamKey songTwoKey = TwoParamKey(bestSong->getNumberOfPlays(), songID);
    if (bestSong->getSongId() == songID) {
        if (artist->getNumberOfSongs() <= 2) {
            // The tree has either 1 or two nodes
            if (artist->getNumberOfSongs() == 1) {
                // The song we are deleting was the only song for this artist, marking best song as empty
                artist->setBestSong(nullptr);
            } else {
                RankTreeNode<TwoParamKey, Song> *bestSongFromPlaysNode = artist->getSongsByPlaysTree().Find(songTwoKey);
                if (bestSongFromPlaysNode->getLeft()) {
                    artist->setBestSong(bestSongFromPlaysNode->getLeft()->getData());
                } else {
                    artist->setBestSong(bestSongFromPlaysNode->getParent()->getData());
                }
            }
        } else {
            // The artist has other songs so now a new song needs to be marked as the artists' best song
            RankTreeNode<TwoParamKey, Song> *bestSongFromPlaysNode = artist->getSongsByPlaysTree().Find(songTwoKey);
            artist->setBestSong(bestSongFromPlaysNode->getParent()->getData());
        }
    }

    artist->getSongsByIdTree().Remove(songID);
    artist->getSongsByPlaysTree().Remove(songTwoKey);
    int oldNumberOfSongsForArtist = artist->getNumberOfSongs();
    artist->setNumberOfSongs(oldNumberOfSongsForArtist - 1);
    numberOfSongs--;
    return SUCCESS;
}

StatusType MusicManager::AddToSongCount(int artistID, int songID, int count) {
    if (numberOfArtists <= 0 || artistHashTable.Find(artistID) != SUCCESS) {
        return FAILURE;
    }

    Artist *artist = artistHashTable.FindNode(artistID)->getData();
    TreeNode<Song> *songNode = artist->getSongsByIdTree().Find(songID);
    if (!songNode) {
        // The song doesn't exist
        return FAILURE;
    }

    Song *song = songNode->getData();
    int oldNumberOfPlays = song->getNumberOfPlays();
    song->setNumberOfPlays(oldNumberOfPlays + count);
    TwoParamKey oldTwoKey = TwoParamKey(oldNumberOfPlays, songID);
    artist->getSongsByPlaysTree().Remove(oldTwoKey);
    TwoParamKey newTwoKey = TwoParamKey(oldNumberOfPlays + count, songID);
    Song *nSongPlays = new Song(songID, artistID, oldNumberOfPlays + count, nullptr);
    if (!nSongPlays) {
        song->setNumberOfPlays(oldNumberOfPlays);
        return ALLOCATION_ERROR;
    }
    artist->getSongsByPlaysTree().Insert(newTwoKey, nSongPlays);
    if (!artist->getSongsByPlaysTree().Find(newTwoKey)) {
        song->setNumberOfPlays(oldNumberOfPlays);
        delete nSongPlays;
        return ALLOCATION_ERROR;
    }

    ThreeParamKey oldThreeKey = ThreeParamKey(oldNumberOfPlays, songID, artistID);
    songRankTree.Remove(oldThreeKey);
    ThreeParamKey newThreeKey = ThreeParamKey(oldNumberOfPlays + count, songID, artistID);
    songRankTree.Insert(newThreeKey);
    return SUCCESS;
}

StatusType MusicManager::GetArtistBestSong(int artistID, int *songID) {
    if (numberOfArtists <= 0 || artistHashTable.Find(artistID) != SUCCESS) {
        return FAILURE;
    }

    Artist *artist = artistHashTable.FindNode(artistID)->getData();
    if (artist->getNumberOfSongs() == 0) {
        return FAILURE;
    }

    *songID = artist->getBestSong()->getSongId();
    return SUCCESS;
}

StatusType MusicManager::GetRecommendedSongInPlace(int rank, int *artistID, int *songID) {
    if (numberOfArtists <= 0 || numberOfSongs < rank) {
        return FAILURE;
    }
    ThreeParamKey found = songRankTree.FindByRank(rank);
    *artistID = found.getArtistId();
    *songID = found.getSongId();

    return SUCCESS;
}

StatusType MusicManager::AddSongToArtist(int artistID, int songID, Artist *artist) {
    Song *nSong = new Song(songID, artistID, 0, nullptr);
    if (!nSong) {
        return ALLOCATION_ERROR;
    }
    artist->getSongsByIdTree().Insert(songID, nSong);
    if (!artist->getSongsByIdTree().Find(songID)) {
        delete nSong;
        return ALLOCATION_ERROR;
    }
    Song *nSongPlays = new Song(songID, artistID, 0, nullptr);
    if (!nSongPlays) {
        artist->getSongsByIdTree().Remove(songID);
        return ALLOCATION_ERROR;
    }
    TwoParamKey songKey = TwoParamKey(0, songID);
    artist->getSongsByPlaysTree().Insert(songKey, nSongPlays);
    if (!artist->getSongsByPlaysTree().Find(songKey)) {
        artist->getSongsByIdTree().Remove(songID);
        return ALLOCATION_ERROR;
    }

    if (artist->getNumberOfSongs() == 0) {
        // This is the first song of the artist, so it is also it's best song.
        artist->setBestSong(nSong);
    } else {
        Song *oldBest = artist->getBestSong();
        if (oldBest->getNumberOfPlays() == 0 && oldBest->getSongId() > songID) {
            // The old best song has 0 plays just like the one we added, so according
            // to printing rules we need to set the new one as the best song.
            artist->setBestSong(nSong);
        }
    }

    if (AddSongToRankTree(artistID, songID, nSong) == ALLOCATION_ERROR) {
        artist->getSongsByIdTree().Remove(songID);
        artist->getSongsByPlaysTree().Remove(songKey);
        return ALLOCATION_ERROR;
    }

    int currentNumberOfSongs = artist->getNumberOfSongs();
    artist->setNumberOfSongs(currentNumberOfSongs + 1);

    return SUCCESS;
}

StatusType MusicManager::AddSongToRankTree(int artistID, int songID, Song *nSong) {
    ThreeParamKey songKey = ThreeParamKey(0, songID, artistID);
    songRankTree.Insert(songKey, nullptr);
    RankTreeNode<ThreeParamKey, int> *nRankedSong = songRankTree.Find(songKey);
    if (!nRankedSong) {
        return ALLOCATION_ERROR;
    }
    nSong->setPtrToRankedSong(nRankedSong);
    return SUCCESS;
}