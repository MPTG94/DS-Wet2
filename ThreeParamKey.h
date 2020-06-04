//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_THREEPARAMKEY_H
#define WET2_THREEPARAMKEY_H


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
};


#endif //WET2_THREEPARAMKEY_H
