//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_THREEPARAMKEY_H
#define WET2_THREEPARAMKEY_H


#include <ostream>

class ThreeParamKey {
private:
    int numberOfPlays = 0;
    int songID = 1;
    int artistID = 1;

public:
    ThreeParamKey();

    ThreeParamKey(int numberOfPlays = 0, int songId = 1, int artistId = 1);

    int getNumberOfPlays() const;

    void setNumberOfPlays(int nNumPlays);

    int getSongId() const;

    void setSongId(int nSongID);

    int getArtistId() const;

    void setArtistId(int nArtistID);

    bool operator==(const ThreeParamKey &other);

    bool operator!=(const ThreeParamKey &other);

    bool operator>(const ThreeParamKey &other);

    bool operator>=(const ThreeParamKey &other);

    bool operator<(const ThreeParamKey &other);

    bool operator<=(const ThreeParamKey &other);

    friend std::ostream &operator<<(std::ostream &os, const ThreeParamKey &key);
};


#endif //WET2_THREEPARAMKEY_H
