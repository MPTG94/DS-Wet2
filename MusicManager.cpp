//
// Created by Mor on 03/06/2020.
//

#include "MusicManager.h"

MusicManager::MusicManager() : artistHashTable(HashTable<Artist>()), songRankTree(AVLRankTree<ThreeParamKey, int>()), numberOfSongs(0),
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
    if (numberOfArtists <= 0 || artistHashTable.FindNode(artistID) == nullptr) {
        return FAILURE;
    }
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
//    songRankTree.RankSanityCheck();

//    Song *bestSong = artist->getBestSong();
//    TwoParamKey songTwoKey = TwoParamKey(bestSong->getNumberOfPlays(), songID);
//    if (bestSong->getSongId() == songID) {
//        if (artist->getNumberOfSongs() <= 2) {
//            // The tree has either 1 or two nodes
//            if (artist->getNumberOfSongs() == 1) {
//                // The song we are deleting was the only song for this artist, marking best song as empty
//                artist->setBestSong(nullptr);
//            } else {
//                RankTreeNode<TwoParamKey, Song> *bestSongFromPlaysNode = artist->getSongsByPlaysTree().Find(songTwoKey);
//                if (bestSongFromPlaysNode->getLeft()) {
//                    // The best song is currently the root of the tree, setting the new best song to be his left son
//                    // (will be fixed after rotations)
//                    artist->setBestSong(bestSongFromPlaysNode->getLeft()->getData());
//                } else {
//                    // The best song is currently the right child of the root, setting the new best song to be his root.
//                    artist->setBestSong(bestSongFromPlaysNode->getParent()->getData());
//                }
//            }
//        } else {
//            // The artist has other songs so now a new song needs to be marked as the artists' best song
//            RankTreeNode<TwoParamKey, Song> *bestSongFromPlaysNode = artist->getSongsByPlaysTree().Find(songTwoKey);
//            artist->setBestSong(bestSongFromPlaysNode->getParent()->getData());
//        }
//    }

    TwoParamKey removedSongKey = TwoParamKey(song->getNumberOfPlays(), songID);
    artist->getSongsByIdTree().Remove(songID);
    artist->getSongsByPlaysTree().Remove(removedSongKey);
    if (artist->getNumberOfSongs() == 1) {
        artist->setBestSong(nullptr);
    } else {
        artist->setBestSong(artist->getSongsByPlaysTree().GetRoot()->findMaxNoRank()->getData());
    }
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
    //artist->compareNumberOfSongs();
    TreeNode<Song> *songNode = artist->getSongsByIdTree().Find(songID);
    if (!songNode) {
        // The song doesn't exist
        return FAILURE;
    }

    Song *songFromId = songNode->getData();
    int oldNumberOfPlays = songFromId->getNumberOfPlays();
    int newNumberOfPlays = oldNumberOfPlays + count;
//    bool isBesPtrReplaced = false;
//    Song *currentBest = artist->getBestSong();
//
//    if (currentBest->getSongId() != songID) {
//        if (currentBest->getNumberOfPlays() < newNumberOfPlays) {
//            isBesPtrReplaced = true;
//        } else if (currentBest->getNumberOfPlays() == newNumberOfPlays && currentBest->getSongId() > songID) {
//            isBesPtrReplaced = true;
//        }
//    } else if (currentBest->getSongId() == songID) {
//        isBesPtrReplaced = true;
//    }

    songFromId->setNumberOfPlays(newNumberOfPlays);
    TwoParamKey oldTwoKey = TwoParamKey(oldNumberOfPlays, songID);
    artist->getSongsByPlaysTree().Remove(oldTwoKey);
    TwoParamKey newTwoKey = TwoParamKey(newNumberOfPlays, songID);
    Song *nSongPlays = new Song(songID, artistID, newNumberOfPlays, nullptr);
    if (!nSongPlays) {
        songFromId->setNumberOfPlays(oldNumberOfPlays);
        return ALLOCATION_ERROR;
    }
    artist->getSongsByPlaysTree().Insert(newTwoKey, nSongPlays);
    if (!artist->getSongsByPlaysTree().Find(newTwoKey)) {
        songFromId->setNumberOfPlays(oldNumberOfPlays);
        delete nSongPlays;
        return ALLOCATION_ERROR;
    }
//    if (isBesPtrReplaced) {
//        artist->setBestSong(nSongPlays);
//    }

    artist->setBestSong(artist->getSongsByPlaysTree().GetRoot()->findMaxNoRank()->getData());

//    songRankTree.RankSanityCheck();
    ThreeParamKey oldThreeKey = ThreeParamKey(oldNumberOfPlays, songID, artistID);
    songRankTree.Remove(oldThreeKey);
    ThreeParamKey newThreeKey = ThreeParamKey(newNumberOfPlays, songID, artistID);
    songRankTree.Insert(newThreeKey, 0);
//    songRankTree.RankSanityCheck();
    //artist->compareNumberOfSongs();
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

//    RankTreeNode<TwoParamKey, Song> *foundMax = artist->getSongsByPlaysTree().GetRoot()->findMaxNoRank();
//    *songID = foundMax->getData()->getSongId();
    *songID = artist->getBestSong()->getSongId();
    return SUCCESS;
}

StatusType MusicManager::GetRecommendedSongInPlace(int rank, int *artistID, int *songID) {
    if (numberOfArtists <= 0 || numberOfSongs < rank) {
        return FAILURE;
    }
    //songRankTree.PrintTreeWithRanks();
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
        delete nSongPlays;
        artist->getSongsByIdTree().Remove(songID);
        return ALLOCATION_ERROR;
    }

    if (artist->getNumberOfSongs() == 0) {
        // This is the first song of the artist, so it is also it's best song.
        artist->setBestSong(nSongPlays);
    } else {
        artist->setBestSong(artist->getSongsByPlaysTree().GetRoot()->findMaxNoRank()->getData());
//        Song *oldBest = artist->getBestSong();
//        if (oldBest->getNumberOfPlays() == 0 && oldBest->getSongId() > songID) {
//            // The old best song has 0 plays just like the one we added, so according
//            // to printing rules we need to set the new one as the best song.
//            artist->setBestSong(nSongPlays);
//        }
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
    songRankTree.Insert(songKey, 0);
//    songRankTree.RankSanityCheck();
//    AVLRankTree<ThreeParamKey>::Node* nRankedSong = songRankTree.getByKey(songKey);
//    if (!nRankedSong) {
//        return ALLOCATION_ERROR;
//    }
    //nSong->setPtrToRankedSong(nRankedSong);
    return SUCCESS;
}