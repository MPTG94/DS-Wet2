//
// Created by Mor on 04/06/2020.
//

#ifndef WET2_TWOPARAMKEY_H
#define WET2_TWOPARAMKEY_H


class TwoParamKey {
private:
    int numberOfPlays = 0;
    int songID = 1;

public:
    TwoParamKey();

    TwoParamKey(int numberOfPlays = 0, int songID = 1);

    int getNumberOfPlays() const;

    void setNumberOfPlays(int nNumPlays);

    int getSongId() const;

    void setSongId(int nSongID);

    bool operator==(const TwoParamKey &other);

    bool operator!=(const TwoParamKey &other);

    bool operator>(const TwoParamKey &other);

    bool operator>=(const TwoParamKey &other);

    bool operator<(const TwoParamKey &other);

    bool operator<=(const TwoParamKey &other);
};


#endif //WET2_TWOPARAMKEY_H
