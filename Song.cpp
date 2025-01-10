
#include "Song.h"
#include <libavformat/avformat.h>

extern "C" {
    #include "leif/leif.h"
    #include <GLFW/glfw3.h>
}

extern Mix_Music* sound;
extern int channel;
extern float position;
extern GLFWwindow* window;

// simple to understand ...  
std::string UpperCase(std::string string)
{
    for (auto& Char : string)
    {
        Char = std::toupper(Char);
    }

    return string;
}

Song::Song( std::string path , std::string song_name , std::string artist , Genre genre)
{
    this->path = path;
    this->song_name = song_name;
    this->artist = artist;
    this->genre = genre;
    time = getAudioDuration();
}

// idk why i made this . . . i just did lol
Song::Song(int None)
{

}

// get the Audio Duration (music length in second)
int Song::getAudioDuration()
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());

    double duration = Mix_MusicDuration(music);
    Mix_FreeMusic(music);
    
    return duration;
}

// load music and set the new title 
void Song::loadAudio()
{
    sound = Mix_LoadMUS(path.c_str());
    std::cout << "loading new music..." << '\n';
    std::cout << "Title : " << song_name << '\n';
    std::cout << "Artist : " << artist << '\n';
    std::cout << "Duration In second : " << time << '\n';
    std::string temp = "ff - " + song_name; 
    glfwSetWindowTitle(window , temp.c_str());
    
}

// play the damn music
void Song::playMusic()
{
    //Mix_PlayMusic(sound,0);
    channel =  Mix_PlayMusic(sound,0);
    paused = false;
}

// free music A.K.A unload the music and resetting the title
void Song::freeMusic()
{
    Mix_FreeMusic(sound);
    glfwSetWindowTitle(window , "ff");
}

// pause and unpause (resume) logic
void Song::pauseMusic()
{
    paused = !paused;
    
    if (paused)
    {
        Mix_PauseMusic();
    }
    else
    {
        Mix_ResumeMusic();
    }

    
}

// set the new title
void Song::changeArtistsName(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setArtist(name.c_str());
    //artist = name;

    f.save();
    
}

// save the new Genre
void Song::changeGenre(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setGenre(name.c_str());

    f.save();
}

void Song::loadAndplay()
{
    loadAudio();
    playMusic();
}

// save the new title
void Song::changeTitlesName(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setTitle(name.c_str());
    //song_name = name;

    f.save();
}

// get the music Genre from a path
std::string GetMusicGenre(std::string path)
{
    TagLib::FileRef file(path.c_str());

    TagLib::Tag *tag = file.tag();

    std::string genre = tag->genre().toCString(true);

    for (auto& c : genre)
        c = toupper(c);

    return genre;

}

// getting the music genre as an enum
Genre GenreStringToEnum(std::string genre)
{
    Genre genre_enum = MUSIC;

    if (genre == "HIP-HOP" || genre == "HIPHOP")
    {
        genre_enum = HIP_HOP;
    }
    else if(genre == "POP")
    {
        genre_enum = POP;
    }
    else if(genre == "KPOP" || genre == "K-POP" || genre == "K_POP")
    {   
        genre_enum = K_POP;
    }
    else if(genre == "COUNTRY")
    {
        genre_enum = COUNTRY;
    }
    else if(genre == "METAL")
    {
        genre_enum = METAL;
    }
    else if(genre == "CHILL")
    {
        genre_enum = CHILL;
    }
    else if(genre == "ROCK")
    {
        genre_enum = ROCK;
    }
    else
    {
        genre_enum = MUSIC;
    }
    
    return genre_enum;
}

