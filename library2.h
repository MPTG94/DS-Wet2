/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1
#define _234218_WET1

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init();

StatusType AddArtist(void *DS, int artistID);

StatusType RemoveArtist(void *DS, int artistID);

StatusType AddSong(void *DS, int artistID, int songID);

StatusType RemoveSong(void *DS, int artistID, int songID);

StatusType AddToSongCount(void *DS, int artistID, int songID, int count);

StatusType GetArtistBestSong(void *DS, int artistID, int *songId);

StatusType GetRecommendedSongInPlace(void *DS, int rank, int *artistId, int *songId);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1 */
