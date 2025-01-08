
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

Song::Song(int None)
{

}

int Song::getAudioDuration()
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());

    double duration = Mix_MusicDuration(music);
    Mix_FreeMusic(music);
    
    return duration;
}

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

void Song::playMusic()
{
    //Mix_PlayMusic(sound,0);
    channel =  Mix_PlayMusic(sound,0);
    paused = false;
}

void Song::freeMusic()
{
    Mix_FreeMusic(sound);
    glfwSetWindowTitle(window , "ff");
}

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

void Song::changeArtistsName(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setArtist(name.c_str());
    //artist = name;

    f.save();
    
}

void Song::changeGenre(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setGenre(name.c_str());

    f.save();
}

void Song::changeTitlesName(std::string name)
{
    TagLib::FileRef f(path.c_str());

    TagLib::Tag* tag = f.tag();

    tag->setTitle(name.c_str());
    //song_name = name;

    f.save();
}

