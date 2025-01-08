
#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <libavformat/avformat.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

// genre enums
enum Genre
{
    MUSIC       = 0  ,
    POP         = 1  ,
    HIP_HOP     = 2  ,
    K_POP       = 3  ,
    ROCK        = 4  ,
    CHILL       = 5  ,
    COUNTRY     = 6  ,
    METAL       = 7  ,
    All         = 8

};

// sort filters
enum SORT_FILTER
{
    MUSIC_TITLE = 0 ,
    MUSIC_ARTIST = 1
};

class Song
{

public:
    std::string path;
    std::string song_name;
    std::string artist;
    Genre genre; // music genre
    int time; // audio duration (music len) in second
    bool paused = true;

    Song( std::string path , std::string song_name , std::string artist , Genre genre);
    Song(int None = -1);
    int getAudioDuration();
    void loadAudio();
    void playMusic();
    void changeArtistsName(std::string name);
    void changeTitlesName(std::string name);
    void changeGenre(std::string name);
    void freeMusic();
    void pauseMusic();
};


std::string UpperCase(std::string string);