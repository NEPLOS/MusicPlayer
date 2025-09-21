
#pragma once

#include "stuff.h"

// basic init stuff
bool initializeSDL() 
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) 
    {
        return false;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) 
    {
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
    {
        return false;
    }

    return true;
}


// read the musics and then sort them all from the file (at the start)
void readFile()
{

    if (std::filesystem::exists( fullHomePath + "musics") && std::filesystem::is_directory( fullHomePath + "music"))
    {
        std::filesystem::create_directory(fullHomePath + "musics");
        return;
    }
    

    for (auto& entry : std::filesystem::directory_iterator(fullHomePath + "musics/")) 
    {
        //std::cerr << "ni\n";
        if (entry.is_regular_file())
        {
           // std::cerr << "yepiiii\n";
            Mix_Music* music = Mix_LoadMUS(entry.path().c_str());

            std::string music_title = Mix_GetMusicTitle(music);
            std::string music_artist = Mix_GetMusicArtistTag(music);
            Genre music_genre = GenreStringToEnum(GetMusicGenre(entry.path().c_str()));

            if (music_artist == "")
            {
               music_artist = "Unknown";
            }

            if (music_title == "")
            {
               music_title = "Unknown";
            }
            

            Song song(entry.path().string() , music_title , music_artist , music_genre);
            std::cerr << song.song_name << '\n';
            playlist->addSong(song);
            Mix_FreeMusic(music);
        }
        
    }
    
}

std::string getFontPath(std::string path)
{
    return (fullHomePath + path);
}

void InItIconsAndFonts()
{

    titleFont = lf_load_font( getFontPath("fonts/1.ttf").c_str() , 35);
    musicNameFont = lf_load_font(getFontPath("fonts/2.ttf").c_str() , 21);
    smaller_musicNameFont = lf_load_font(getFontPath("fonts/2.ttf").c_str() , 18);

    pauseIcon = lf_load_texture   (getFontPath("icons/pause.png").c_str(),true , LF_TEX_FILTER_LINEAR);
    playIcon = lf_load_texture    (getFontPath("icons/play.png").c_str(),true , LF_TEX_FILTER_LINEAR);
    backIcon = lf_load_texture    (getFontPath("icons/back.png").c_str(),true , LF_TEX_FILTER_LINEAR);
    trashbinIcon = lf_load_texture(getFontPath("icons/trashbin.png").c_str(),true , LF_TEX_FILTER_LINEAR);
    searchIcon = lf_load_texture  (getFontPath("icons/search.png").c_str(),true , LF_TEX_FILTER_LINEAR);
    previousIcon = lf_load_texture(getFontPath("icons/previous.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    nextIcon = lf_load_texture    (getFontPath("icons/next.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    volumeIcon = lf_load_texture  (getFontPath("icons/volume.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    muteIcon = lf_load_texture    (getFontPath("icons/mute.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    shuffleIcon = lf_load_texture (getFontPath("icons/shuffle.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    loopIcon = lf_load_texture    (getFontPath("icons/loop.png").c_str(),true,LF_TEX_FILTER_LINEAR);
    loopOneIcon = lf_load_texture (getFontPath("icons/loop_one.png").c_str(),true,LF_TEX_FILTER_LINEAR);

    new_task_input_path.buf_size = 512;
    
    memset(input_str_path, 0, 512);
        new_task_input_path = (LfInputField)
        {
            .width = 400,
            .buf = input_str_path,
            .buf_size = 512,
            .placeholder = (char*)"file path "
        };

    new_task_input_artist.buf_size = 512;

    //------------------------------------------

    memset(input_str_artist, 0, 512);
    new_task_input_artist = (LfInputField)
    {
        .width = 400,
        .buf = input_str_artist,
        .buf_size = 512,
        .placeholder = (char*)"artist"
    };

    //------------------------------------------

    new_task_input_title.buf_size = 512;

    memset(input_str_title, 0, 512);
    new_task_input_title = (LfInputField)
    {
        .width = 400,
        .buf = input_str_title,
        .buf_size = 512,
        .placeholder = (char*)"title"
    };

    //------------------------------------------
    new_task_input_title_search.buf_size = 512;

    memset(input_str_title_search, 0, 512);
    new_task_input_title_search = (LfInputField)
    {
        .width = 300,
        .buf = input_str_title_search,
        .buf_size = 512,
        .placeholder = (char*)"title search"
    };

    //------------------------------------------

    new_task_input_artist_search.buf_size = 512;

    memset(input_str_artist_search, 0, 512);
    new_task_input_artist_search = (LfInputField)
    {
        .width = 300,
        .buf = input_str_artist_search,
        .buf_size = 512,
        .placeholder = (char*)"artist search"
    };

}

// free the memory (texture and fonts)
void free_memory()
{
    lf_free_texture(&playIcon);
    lf_free_texture(&pauseIcon);
    lf_free_texture(&backIcon);
    lf_free_texture(&trashbinIcon);
    lf_free_texture(&searchIcon);
    lf_free_texture(&nextIcon);
    lf_free_texture(&previousIcon);
    lf_free_texture(&volumeIcon);
    lf_free_texture(&muteIcon);
    lf_free_texture(&shuffleIcon);
    lf_free_texture(&loopIcon);
    lf_free_texture(&loopOneIcon);


    lf_free_font(&titleFont);
    lf_free_font(&musicNameFont);
    lf_free_font(&smaller_musicNameFont);

}
