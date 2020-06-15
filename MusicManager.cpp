//
// Created by Mor on 03/06/2020.
//

#include "MusicManager.h"

MusicManager::MusicManager() : artistHashTable(HashTable<Artist>()), songRankTree(RankTreeSingle<ThreeParamKey>()), numberOfSongs(0),
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
    TreeNode<Song> *songNode = artist->getSongsByIdTree().Find(songID);
    if (!songNode) {
        // The song doesn't exist
        return FAILURE;
    }

    Song *songFromId = songNode->getData();
    int oldNumberOfPlays = songFromId->getNumberOfPlays();
    int newNumberOfPlays = oldNumberOfPlays + count;

    songFromId->setNumberOfPlays(newNumberOfPlays);
    TwoParamKey oldTwoKey = TwoParamKey(oldNumberOfPlays, songID);
    artist->getSongsByPlaysTree().Remove(oldTwoKey);
    TwoParamKey newTwoKey = TwoParamKey(newNumberOfPlays, songID);
    Song *nSongPlays = new Song(songID, artistID, newNumberOfPlays);
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

    artist->setBestSong(artist->getSongsByPlaysTree().GetRoot()->findMaxNoRank()->getData());

    ThreeParamKey oldThreeKey = ThreeParamKey(oldNumberOfPlays, songID, artistID);
    songRankTree.Remove(oldThreeKey);
    ThreeParamKey newThreeKey = ThreeParamKey(newNumberOfPlays, songID, artistID);
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
    Song *nSong = new Song(songID, artistID, 0);
    if (!nSong) {
        return ALLOCATION_ERROR;
    }
    artist->getSongsByIdTree().Insert(songID, nSong);
    if (!artist->getSongsByIdTree().Find(songID)) {
        delete nSong;
        return ALLOCATION_ERROR;
    }
    Song *nSongPlays = new Song(songID, artistID, 0);
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
    songRankTree.Insert(songKey);
    if (!songRankTree.Find(songKey)) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}