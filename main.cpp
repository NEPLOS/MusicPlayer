
#include <iostream>
#include "linked_list.h"
#include "Song.h"
#include <filesystem>


extern "C" {
    #include "leif/leif.h"
    #include <GLFW/glfw3.h>
    #include "tinyfiledialogs.h"
}

#define MARGIN 20

#define ONE_MUSIC 0
#define ALL_MUSICS 1


enum Genre;

GLFWwindow* window;

static LfFont titleFont;
static LfFont musicNameFont;
static LfFont smaller_musicNameFont;

Mix_Music* sound = nullptr;
int channel = -1;
float position = 0;

const int width = 1100;
const int hight = 650;

bool space_key = true;
bool m_key = true;

DoubleLinkedList* playlist = nullptr;

static bool is_open_genre = false;

static LfInputField new_task_input_path;
static LfInputField new_task_input_artist;
static LfInputField new_task_input_title;
static LfInputField new_task_input_artist_search;
static LfInputField new_task_input_title_search;

static char input_str_path[512];
static char input_str_artist[512];
static char input_str_title[512];
static char input_str_artist_search[512];
static char input_str_title_search[512];

static LfTexture playIcon;
static LfTexture pauseIcon;
static LfTexture backIcon;
static LfTexture trashbinIcon;
static LfTexture searchIcon;
static LfTexture nextIcon;
static LfTexture previousIcon;
static LfTexture volumeIcon;
static LfTexture muteIcon;
static LfTexture shuffleIcon;
static LfTexture loopIcon;
static LfTexture loopOneIcon;
//static LfTexture muteIcon;

Node* playing_song = nullptr;

static bool is_mute = false;
static int last_audio = MIX_MAX_VOLUME / 2;

bool current_sort = true;

enum Tab
{
    NONE = 0 ,
    MAIN_PAGE = 1 ,
    ADD_NEW_SONG = 2 ,
    EDIT_PAGE = 3

};

Genre Filter = All;

Tab current_tab = MAIN_PAGE;

static bool loop_type = ONE_MUSIC;

static bool should_i_pause_or_unpause_the_music = true;
static bool should_i_mute_or_unmute_the_music = true;


std::string GetMusicGenre(std::string path)
{
    TagLib::FileRef file(path.c_str());

    TagLib::Tag *tag = file.tag();

    std::string genre = tag->genre().toCString(true);

    for (auto& c : genre)
        c = toupper(c);

    return genre;

}

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

void TopUI()
{
    lf_push_font(&titleFont);
    lf_text("Music Program");
    lf_pop_font();

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().inputfield_props;
        props.padding = 15;
        props.border_width = 0;
        props.margin_right = 15;
        props.margin_left = 37;
        props.color = ((LfColor){30 , 30 , 30 , 30});
        props.text_color = LF_WHITE;
        props.border_width = 1.0f;
        props.border_color = new_task_input_title_search.selected ? LF_WHITE : (LfColor){140, 140, 140, 255};
        should_i_pause_or_unpause_the_music = !(new_task_input_title_search.selected);
        should_i_mute_or_unmute_the_music = should_i_pause_or_unpause_the_music;
        props.corner_radius = 1.5f;
        props.margin_bottom = 10.0f;
        lf_push_style_props(props);
        lf_input_text(&new_task_input_title_search);
        lf_pop_style_props();
        lf_pop_font();
    }

    {
        lf_set_ptr_x_absolute(width - 160 - MARGIN * 2);
        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = 0;
        props.margin_right = 0;
        props.color = (LfColor){65, 167, 204 ,255};
        props.border_width = 0;
        props.corner_radius = 4;
        lf_push_style_props(props);
        //lf_set_line_should_overflow(false);
        if(lf_button_fixed("Add New Music" , 160 , -1) == LF_CLICKED)
        {
            current_tab = ADD_NEW_SONG;
        }

        lf_pop_style_props();
        //lf_set_line_should_overflow(true);
    }

    lf_next_line();


    //lf_set_ptr_x_absolute(400);
    //lf_set_ptr_y_absolute(200);

    
    {
        static int selected_genre = 0;

        //lf_set_line_should_overflow(true);

        static const char* items[] = { "All" , "music" , "pop" , "hip-hop" , "k-pop" , "rock" , "chill" , "country" , "metal"}; 

        {
            LfUIElementProps props = lf_get_theme().button_props;
            props.color = (LfColor){18 , 18 , 18 , 255};
            //props.margin_left = 15;
            props.border_width = 0;
            props.text_color = LF_WHITE;
            //props.corner_radius = 0;
            props.border_color = (LfColor){180 , 180 , 180 , 255};
            lf_push_font(&musicNameFont);
            lf_push_style_props(props);
            lf_dropdown_menu(items , "Genre : " , 9 , 170 , 230 , &selected_genre , &is_open_genre);
            lf_pop_style_props();
            lf_pop_font();

            if (selected_genre == 0)
            {
                Filter = All;
            }
            else
            {
                Filter = (Genre)(selected_genre - 1);
            }
            
        }

        //lf_set_line_should_overflow(false);
    }

    //lf_next_line();

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().inputfield_props;
        props.padding = 15;
        props.margin_right = 15;
        props.margin_left = 15;
        props.border_width = 0;
        props.color = ((LfColor){30 , 30 , 30 , 30});
        props.text_color = LF_WHITE;
        props.border_width = 1.0f;
        props.border_color = new_task_input_artist_search.selected ? LF_WHITE : (LfColor){140, 140, 140, 255};

        if(should_i_pause_or_unpause_the_music)
            should_i_pause_or_unpause_the_music = !new_task_input_artist_search.selected;
        should_i_mute_or_unmute_the_music = should_i_pause_or_unpause_the_music;

        props.corner_radius = 1.5f;
        props.margin_bottom = 10.0f;
        lf_push_style_props(props);
        lf_input_text(&new_task_input_artist_search);
        lf_pop_style_props();
        lf_pop_font();
    }

    {
        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = -10;
        props.color = LF_NO_COLOR;
        props.border_width = 0;

        lf_push_style_props(props);

        if (lf_image_button(((LfTexture){.id = trashbinIcon.id , 24 , 24})) == LF_CLICKED)
        {   
            if (strlen(input_str_artist_search) != 0)
            {
                playlist->removeSong_by_artist_search(input_str_artist_search);

                if (playing_song != nullptr)
                {
                    playing_song = playing_song->next;
                    Mix_FreeMusic(sound);

                    if (playing_song != nullptr)
                    {
                        playing_song->song.loadAudio();
                        playing_song->song.playMusic();
                        space_key = true;
                    }
                }
            }
            
            
        }

        lf_pop_style_props();

    }

    //lf_set_ptr_x_absolute(lf_get_ptr_x() + 70);

    {
        lf_set_ptr_x_absolute(width - 160 - MARGIN * 2);
        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = -8;
        props.margin_top = 7;
        //props.margin_right = 70;
        props.color = (LfColor){65, 167, 204 ,255};
        props.border_width = 0;
        props.corner_radius = 4;
        lf_push_style_props(props);
        //lf_set_line_should_overflow(false);

        std::string name = (current_sort)? "Artist" : "Title";

        name = "sort base on " + name;

        if(lf_button_fixed(name.c_str() , 176 , -1) == LF_CLICKED)
        {
            current_sort = !current_sort;

            (current_sort)? playlist->Sort(MUSIC_TITLE) : playlist->Sort(MUSIC_ARTIST);            

        }

        lf_pop_style_props();
        //lf_set_line_should_overflow(true);
    }

    //lf_next_line();
    //lf_next_line();

    if(playing_song != nullptr)
    {
        lf_set_ptr_x_absolute((width - MARGIN * 2 - 300) / 2 + 20);
        lf_set_ptr_y_absolute(430);
        lf_push_font(&musicNameFont);

        std::string neplos = " * " + playing_song->song.song_name + "     " + std::to_string((int)(Mix_GetMusicPosition(sound))) + " | " + std::to_string(playing_song->song.time);

        lf_text(neplos.c_str());
        lf_pop_font();
    }

    lf_next_line();

}

void sidePanel()
{
 //   std::cerr << "hello\n";

    if (playlist->head == nullptr)
    {
        lf_push_font(&musicNameFont);
        lf_text("no Musics :(( ");
        lf_next_line();
        lf_text("please Add Music :D ");
        lf_pop_font();

        return;
    }
    
    Node* current = playlist->head;

    int count = 0;
   // std::cerr << "hello\n";
    while (current)
    {
        // int box_x = lf_get_ptr_x();
        // int box_y = lf_get_ptr_y();

        // lf_rect(300 - MARGIN * 2 - 10, 70 ,LF_NO_COLOR,0);

        if (Filter != All && current->song.genre != Filter)
        {
            current = current->next;
            continue;
        }

        // lf_set_ptr_x_absolute(box_x);
        // lf_set_ptr_y_absolute(box_y);

        {
            std::string title_song = current->song.song_name;
            std::string input_title_song = input_str_title_search;
            //std::cout << input_title_song.size() << '\n';
            // for (auto& c : title_song)
            // {
            //     c = toupper(c);
            // }
            
            title_song = UpperCase(title_song);
            input_title_song = UpperCase(input_title_song);

            // for (auto& c : input_title_song)
            // {
            //     c = toupper(c);
            // }

            //std::cout << title_song << " -- " << input_title_song << '\n';

            if (!input_title_song.empty() && title_song.find(input_title_song) == std::string::npos)
            {
                current = current->next;
                continue;
            }
            
        }

        {
            std::string artist_song = current->song.artist;
            std::string input_artist_song = input_str_artist_search;

            for (auto& c : artist_song)
            {
                c = toupper(c);
            }

            for (auto& c : input_artist_song)
            {
                c = toupper(c);
            }

            if (!input_artist_song.empty() && artist_song.find(input_artist_song) == std::string::npos)
            {
                current = current->next;
                continue;
            }
            
        }


        //std::cerr << "hello\n";
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;

        lf_push_style_props(props);

        //std::cerr << ii << " : " << current->song.paused << '\n';

        if (current->song.paused == true)
        {

            // if(Mix_Playing(-1) == 0)
            // {
            //     Mix_SetMusicPosition(0);
            // }

            if ((Mix_PlayingMusic() == 0) && (playing_song != nullptr) && (sound != nullptr) && Mix_Playing(-1) == 0)
            {
                if (loop_type)
                {
                    if(playing_song->next != nullptr)
                    {
                        if(!(playing_song->song.paused)) playing_song->song.paused = true;
                        playing_song = playing_song->next;
                        playing_song->song.loadAudio();
                        playing_song->song.playMusic();
                    }
                    else
                    {
                         if(!(playing_song->song.paused)) playing_song->song.paused = true;
                        playing_song = nullptr;
                        glfwSetWindowTitle(window , "ff");
                        Mix_FreeMusic(sound);
                    }
                }
                else
                {
                    playing_song->song.loadAudio();
                    playing_song->song.playMusic();
                }

            }
            

            if (lf_image_button(((LfTexture){.id = playIcon.id , 24 , 24})) == LF_CLICKED)
            {   
                std::cerr << "hello 1\n";

                if(playing_song == current)
                {
                    current->song.pauseMusic();
                }
                else
                {
                    std::cerr << "hello 2\n";

                    if (playing_song != nullptr) 
                    {

                        playing_song->song.pauseMusic();
                        Mix_FreeMusic(sound);
                        sound = nullptr;
                        playing_song->song.paused = true;
                    }

                    playing_song = current;
                    std::cerr << "hello 3\n";
                    playing_song->song.loadAudio();
                    playing_song->song.playMusic();
                    current->song.paused = false;
                    std::cerr << "hello 4\n";

                }
                
            }
        }
        else
        {
            if (lf_image_button(((LfTexture){.id = pauseIcon.id , 24 , 24})) == LF_CLICKED)
            {
                current->song.pauseMusic();
            }
        }
        
        
        lf_pop_style_props();

        //smaller_musicNameFont

        float current_x = lf_get_ptr_x();
        {
            lf_set_line_should_overflow(false);

            lf_set_ptr_y_absolute(lf_get_ptr_y() + 7);
            lf_push_font(&musicNameFont);
            //char name[50];
            //sscanf(current->song.song_name.c_str(), "musics/%49s", name);
            std::string nero = current->song.song_name;



            if (nero.size() > 15) {
                nero = nero.substr(0, 15 - 3) + "...";
            }


            //std::string nero = nero + " : " + std::to_string(current->song.time);
            lf_set_text_wrap(true);
            lf_text(nero.c_str());
            lf_set_text_wrap(false);

            lf_set_ptr_y_absolute(lf_get_ptr_y() + 25);
            lf_set_ptr_x_absolute(current_x + 3);
            
            lf_pop_font();
            lf_set_line_should_overflow(true);
        }
        
        {
            LfUIElementProps props = lf_get_theme().button_props;
            props.margin_left = 3;
            props.color = LF_NO_COLOR;
            props.border_width = 0;

            lf_set_ptr_y_absolute(lf_get_ptr_y() - 33);
            lf_set_ptr_x_absolute(current_x + 150);

            lf_push_style_props(props);

            if (lf_image_button(((LfTexture){.id = trashbinIcon.id , 24 , 24})) == LF_CLICKED)
            {   
                playlist->removeSonge_by_name(current->song.song_name);

                if (playing_song == current)
                {
                    playing_song = playing_song->next;
                    Mix_FreeMusic(sound);

                    if (playing_song != nullptr)
                    {
                        playing_song->song.loadAudio();
                        playing_song->song.playMusic();
                        space_key = true;
                    }
                }
                
                
            }

            lf_pop_style_props();

        }

        lf_set_ptr_y_absolute(lf_get_ptr_y() + 30);
        lf_set_ptr_x_absolute(current_x + 2);

        {
//            lf_next_line();

            lf_push_font(&smaller_musicNameFont);
            lf_text(current->song.artist.c_str());
            lf_pop_font();

            lf_next_line();
            //lf_set_line_should_overflow(true);
            current = current->next;
        }


        count++;
    }

    if(count == 0)
    {
        lf_push_font(&musicNameFont);
        lf_text("no Music found :(( ");
        lf_pop_font();

        return;
    }

}

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

void bottomPanel()
{
    if (playing_song == nullptr)
    {
        return;
    }

    bool key = lf_key_is_down(32);

    if (key && space_key && should_i_pause_or_unpause_the_music)
    {
        playing_song->song.pauseMusic();
        std::cerr << "you pressed space\n";
        space_key = false;
    }

    if (lf_key_is_released(32))
    {
        space_key = true;
    }
    
    // Mix_GetMusicVolume(sound);

    int herbert = (Mix_GetMusicPosition(sound) * 100)/((float)(playing_song->song.time));

    herbert = herbert * 5;

    float current_x = lf_get_ptr_x();


    float slider_y = lf_get_ptr_y();

    {
        float slider_x = lf_get_ptr_x();
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_BLACK;
        props.border_color = LF_NO_COLOR;
        props.hover_color = LF_NO_COLOR;
        props.text_color = LF_NO_COLOR;
        props.border_width = 9;

        props.margin_right = 0;
        props.margin_bottom = 0;
        lf_push_style_props(props);
        lf_set_ptr_x_absolute(lf_get_ptr_x() + 120);
        lf_set_ptr_y_absolute(lf_get_ptr_y() - 13);
        slider_x = slider_x + 120;


        //lf_rect( 500 ,3,LF_BLACK , 1);
        if(lf_button_fixed("" , 500 , 1) == LF_CLICKED)
        {
        
            float mouse_x = lf_mouse_move_event().x;

            if (mouse_x >= slider_x && mouse_x <= slider_x + 500) 
            {
                herbert = (mouse_x - slider_x) - 5;

                if (herbert < 0) herbert = 0;
                if (herbert > 505) herbert = 505;
                Mix_SetMusicPosition((playing_song->song.time * (herbert/5)) / 100);
            }
            
        }

        lf_pop_style_props();
    }


    lf_set_ptr_x_absolute(current_x + 130);
    lf_set_ptr_y_absolute(slider_y);

    lf_rect(herbert,3,LF_WHITE,1);
   

    lf_set_ptr_y_absolute(slider_y + 24);

    {
        lf_set_ptr_x_absolute(556);

        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);


        if (lf_image_button(((LfTexture){.id = shuffleIcon.id , 16 , 16})) == LF_CLICKED)
        {   
            // if (playing_song->pre != nullptr)
            // {
            //     if(!playing_song->song.paused) playing_song->song.pauseMusic();
            //     Mix_FreeMusic(sound);
            //     playing_song = playing_song->pre;
            //     playing_song->song.loadAudio();
            //     playing_song->song.playMusic();
            // }
            // else
            // {
            //     Mix_FreeMusic(sound);
            //     playing_song->song.loadAudio();
            //     playing_song->song.playMusic();
            // }

            Node* temp = playing_song;

            int number = rand() % 100;

            for (int i = 0; i <= number; i++)
            {
                if(temp->next == nullptr)
                {
                    temp = playlist->head;
                }
                else
                {
                    temp = temp->next;
                }
            }
            
            if(!playing_song->song.paused) playing_song->song.pauseMusic();
            playing_song->song.freeMusic();
            //Mix_FreeMusic(sound);
            playing_song = temp;
            playing_song->song.loadAudio();
            playing_song->song.playMusic();
            
            
        }


        lf_pop_style_props();
    }

    lf_set_ptr_y_absolute(slider_y + 16);

    {

        lf_set_ptr_x_absolute(596);

        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);


        if (lf_image_button(((LfTexture){.id = previousIcon.id , 32 , 32})) == LF_CLICKED)
        {   
            if (playing_song->pre != nullptr)
            {
                if(!playing_song->song.paused) playing_song->song.pauseMusic();
                playing_song->song.freeMusic();
                //Mix_FreeMusic(sound);
                playing_song = playing_song->pre;
                playing_song->song.loadAudio();
                playing_song->song.playMusic();
            }
            else
            {
                playing_song->song.freeMusic();
                //Mix_FreeMusic(sound);
                playing_song->song.loadAudio();
                playing_song->song.playMusic();
            }
            
            
        }


        lf_pop_style_props();

    }

    lf_set_ptr_y_absolute(slider_y);
    
    {

        lf_set_ptr_x_absolute(648);

        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);

        if (playing_song->song.paused == true)
        {

            if (lf_image_button(((LfTexture){.id = playIcon.id , 64 , 64})) == LF_CLICKED)
            {   
                playing_song->song.pauseMusic();
                
            }
        }
        else
        {
            if (lf_image_button(((LfTexture){.id = pauseIcon.id , 64 , 64})) == LF_CLICKED)
            {
                playing_song->song.pauseMusic();
            }
        }

        lf_pop_style_props();

    }

    lf_set_ptr_y_absolute(slider_y + 16);

    {

        lf_set_ptr_x_absolute(730);

        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);


        if (lf_image_button(((LfTexture){.id = nextIcon.id , 32 , 32})) == LF_CLICKED)
        {   
            if (playing_song->next != nullptr)
            {
                if(!playing_song->song.paused) playing_song->song.pauseMusic();
                playing_song->song.freeMusic();
                //Mix_FreeMusic(sound);
                playing_song = playing_song->next;
                playing_song->song.loadAudio();
                playing_song->song.playMusic();
            }
            else
            {
                //Mix_FreeMusic(sound);
                playing_song->song.freeMusic();
                playing_song->song.loadAudio();
                playing_song->song.playMusic();
            }
            
        }


        lf_pop_style_props();

    }

    lf_set_ptr_y_absolute(slider_y + 21);

    int temp_x = lf_get_ptr_x();

    {
        lf_set_ptr_x_absolute(786);

        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);

        
        if (loop_type)
        {
            if (lf_image_button(((LfTexture){.id = loopIcon.id , 24 , 24})) == LF_CLICKED)
            {   
                loop_type = !loop_type;
            }
        }
        else
        {
            if (lf_image_button(((LfTexture){.id = loopOneIcon.id , 24 , 24})) == LF_CLICKED)
            {   
                loop_type = !loop_type;
            }
        }


        lf_pop_style_props();
    }


    lf_set_ptr_y_absolute(hight - MARGIN * 2 - 32);
    lf_set_ptr_x_absolute(temp_x + 125);

    {
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;
        lf_push_style_props(props);


        // bool key = lf_key_is_down(32);

        // if (key && space_key && should_i_pause_or_unpause_the_music)
        // {
        //     playing_song->song.pauseMusic();
        //     std::cerr << "you pressed space\n";
        //     space_key = false;
        // }

        // if (lf_key_is_released(32))
        // {
        //     space_key = true;
        // }

        bool key = lf_key_went_down(77);

        if(key && should_i_mute_or_unmute_the_music)
        {
            std::cout << "you pressed M \n";
        }

        if (is_mute)
        {
            if (lf_image_button(((LfTexture){.id = muteIcon.id , 32 , 32})) == LF_CLICKED || (key && should_i_mute_or_unmute_the_music))
            {   
                Mix_VolumeMusic(last_audio);
                is_mute = !is_mute;
                m_key = false;
            }
        }
        else
        {
            if (lf_image_button(((LfTexture){.id = volumeIcon.id , 32 , 32})) == LF_CLICKED || (key && should_i_mute_or_unmute_the_music))
            {   
                last_audio = Mix_GetMusicVolume(sound);
                Mix_VolumeMusic(0);
                is_mute = !is_mute;
                m_key = false;
            }
        }

        if (lf_key_is_released(77))
        {
            m_key = true;
        }


        lf_pop_style_props();
    }

    int musicVolume = Mix_GetMusicVolume(sound) * 100 / MIX_MAX_VOLUME;

    //musicVolume = musicVolume * 3;

    lf_set_ptr_y_absolute(hight - MARGIN * 2 - 3);
    lf_set_ptr_x_absolute(lf_get_ptr_x() - 135);
    
   // lf_next_line();

    float current_x_2 = lf_get_ptr_x();
    

    float slider_y_2 = lf_get_ptr_y();

    {
        float slider_x = lf_get_ptr_x();
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_BLACK;
        props.border_color = LF_NO_COLOR;
        props.hover_color = LF_NO_COLOR;
        props.text_color = LF_NO_COLOR;
        props.border_width = 9;

        props.margin_right = 0;
        props.margin_bottom = 0;
        lf_push_style_props(props);
        lf_set_ptr_x_absolute(lf_get_ptr_x() + 120);
        lf_set_ptr_y_absolute(lf_get_ptr_y() - 13);
        slider_x = slider_x + 120;


        //lf_rect( 500 ,3,LF_BLACK , 1);
        if(lf_button_fixed("" , 100 , 1) == LF_CLICKED)
        {

            is_mute = false;

            {
                float mouse_x = lf_mouse_move_event().x;

                if (mouse_x >= slider_x && mouse_x <= slider_x + 500) 
                {
                    musicVolume = (mouse_x - slider_x) - 5;

                    if (musicVolume < 0) musicVolume = 0;
                    if (musicVolume > 105) musicVolume = 105;
                    Mix_VolumeMusic(musicVolume * MIX_MAX_VOLUME / 100);
                }
            }
        }

        lf_pop_style_props();
    }


    lf_set_ptr_x_absolute(current_x_2 + 130);
    lf_set_ptr_y_absolute(slider_y_2);

    lf_rect(musicVolume,3,LF_WHITE,1);


}

void readFile()
{

    if (std::filesystem::exists("musics") & std::filesystem::is_directory("music"))
    {
        std::filesystem::create_directory("musics");
        return;
    }
    

    for (auto& entry : std::filesystem::directory_iterator("musics/")) 
    {
        std::cerr << "ni\n";
        if (entry.is_regular_file())
        {
            std::cerr << "yepiiii\n";
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
            //song.changeArtistsName("r");
            std::cerr << song.song_name << '\n';
            //song.changeAlbumsName("r");
            playlist->addSong(song);
            Mix_FreeMusic(music);
        }
        
    }

    playlist->Sort(MUSIC_TITLE);
    
}

void InItIconsAndFonts()
{

    titleFont = lf_load_font("fonts/1.ttf" , 35);
    musicNameFont = lf_load_font("fonts/2.ttf" , 21);
    smaller_musicNameFont = lf_load_font("fonts/2.ttf" , 18);

    pauseIcon = lf_load_texture("./icons/pause.png",true , LF_TEX_FILTER_LINEAR);
    playIcon = lf_load_texture("./icons/play.png",true , LF_TEX_FILTER_LINEAR);
    backIcon = lf_load_texture("./icons/back.png",true , LF_TEX_FILTER_LINEAR);
    trashbinIcon = lf_load_texture("./icons/trashbin.png",true , LF_TEX_FILTER_LINEAR);
    searchIcon = lf_load_texture("./icons/search.png",true , LF_TEX_FILTER_LINEAR);
    previousIcon = lf_load_texture("./icons/previous.png",true,LF_TEX_FILTER_LINEAR);
    nextIcon = lf_load_texture("./icons/next.png",true,LF_TEX_FILTER_LINEAR);
    volumeIcon = lf_load_texture("./icons/volume.png",true,LF_TEX_FILTER_LINEAR);
    muteIcon = lf_load_texture("./icons/mute.png",true,LF_TEX_FILTER_LINEAR);
    shuffleIcon = lf_load_texture("./icons/shuffle.png",true,LF_TEX_FILTER_LINEAR);
    loopIcon = lf_load_texture("./icons/loop.png",true,LF_TEX_FILTER_LINEAR);
    loopOneIcon = lf_load_texture("./icons/loop_one.png",true,LF_TEX_FILTER_LINEAR);

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

void RenderMainPage()
{
    lf_div_begin(((vec2s){300 , MARGIN}) , ((vec2s){ width - MARGIN * 2 ,500}) , false );

    TopUI(); // the music image , visulizer , anything , whatever , i have no idea what to put in there , why r u still reading this ? 

    lf_div_end();


    {
        lf_set_ptr_x_absolute(285);
        lf_set_ptr_y_absolute(0);

        lf_rect(1,hight,LF_WHITE,0);

    }


    lf_div_begin(((vec2s){MARGIN , MARGIN}) , ((vec2s){280 , hight - MARGIN * 2}) , true);
    
    sidePanel(); // panel , side panel
    
    lf_div_end();


    lf_div_begin(((vec2s){300 , 498}) , ((vec2s){width - MARGIN * 2 , hight - MARGIN * 2}) , false);

    bottomPanel(); // botten , botten panel   //lf_rect(100,100,LF_WHITE,0);

    lf_div_end();

    {
        lf_set_ptr_x_absolute(285);
        lf_set_ptr_y_absolute(0);             // big white line

        lf_rect(1,hight,LF_WHITE,0);

    }

}

void RenderAddNewSongPage()
{
    lf_div_begin(((vec2s){MARGIN , MARGIN}) , ((vec2s){ width - MARGIN * 2 ,hight - MARGIN * 2}) , true );

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.color = (LfColor){200 , 200 , 200 , 200};
        props.margin_bottom = 20;
        lf_push_style_props(props);
        lf_text("Add a new Music");
        lf_pop_style_props();
        lf_pop_font();
    }

    lf_next_line();

    {

        lf_push_font(&smaller_musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.margin_left = 15;
        props.text_color = (LfColor){148, 148, 148 , 255};
        lf_push_style_props(props);
        //lf_text_wide("music path :");
        lf_text("Music path :");
        lf_pop_style_props();
        lf_pop_font();

    }

    lf_next_line();

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().inputfield_props;
        props.padding = 15;
        props.border_width = 0;
        props.color = ((LfColor){50 , 50 , 50 , 50});
        props.text_color = LF_WHITE;
        props.border_width = 1.0f;
        props.border_color = new_task_input_path.selected ? LF_WHITE : (LfColor){170, 170, 170, 255};
        props.corner_radius = 2.5f;
        props.margin_bottom = 10.0f;
        lf_push_style_props(props);
        lf_input_text(&new_task_input_path);
        lf_pop_style_props();
        lf_pop_font();
    }

    {
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = (LfColor){90 , 90 , 90, 255};
        props.border_width = 1;
        props.margin_right = 10;
        props.corner_radius = 4;
        props.border_color = (LfColor){1 , 1 , 1, 255};
        //props.margin_bottom = 10;

        lf_push_style_props(props);

        if (lf_image_button(((LfTexture){.id = searchIcon.id , 24 , 24})) == LF_CLICKED)
        {
            const char* filters[1] = {"*.mp3"};

            const char* filePath = tinyfd_openFileDialog(
                "Select an MP3 File",
                "",
                1,
                filters,
                "MP3 Files",
                0
            );

            if (filePath) 
            {
                Mix_Music* music = Mix_LoadMUS(filePath);

                std::string music_title = Mix_GetMusicTitle(music);
                std::string music_artist = Mix_GetMusicArtistTag(music);

              //  input_str_path = filePath;        

                strcpy(input_str_path , filePath);
                strcpy(input_str_artist, music_artist.c_str());
                strcpy(input_str_title , music_title.c_str());

                Mix_FreeMusic(music);
            } 

        }
    }
    lf_next_line();

    {

        lf_push_font(&smaller_musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.margin_left = 15;
        props.text_color = (LfColor){148, 148, 148 , 255};
        lf_push_style_props(props);
        //lf_text_wide("music path :");
        lf_text("Title :");
        lf_pop_style_props();
        lf_pop_font();

    }

    lf_next_line();

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().inputfield_props;
        props.padding = 15;
        props.border_width = 0;
        props.color = ((LfColor){50 , 50 , 50 , 50});
        props.text_color = LF_WHITE;
        props.border_width = 1.0f;
        props.border_color = new_task_input_title.selected ? LF_WHITE : (LfColor){170, 170, 170, 255};
        props.corner_radius = 2.5f;
        props.margin_bottom = 10.0f;
        lf_push_style_props(props);
        lf_input_text(&new_task_input_title);
        lf_pop_style_props();
        lf_pop_font();
    }
    lf_next_line();

    {

        lf_push_font(&smaller_musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.margin_left = 15;
        props.text_color = (LfColor){148, 148, 148 , 255};
        lf_push_style_props(props);
        //lf_text_wide("music path :");
        lf_text("Artist :");
        lf_pop_style_props();
        lf_pop_font();

    }

    lf_next_line();

    {
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().inputfield_props;
        props.padding = 15;
        props.border_width = 0;
        props.color = ((LfColor){50 , 50 , 50 , 50});
        props.text_color = LF_WHITE;
        props.border_width = 1.0f;
        props.border_color = new_task_input_artist.selected ? LF_WHITE : (LfColor){170, 170, 170, 255};
        props.corner_radius = 2.5f;
        props.margin_bottom = 10.0f;
        lf_push_style_props(props);
        lf_input_text(&new_task_input_artist);
        lf_pop_style_props();
        lf_pop_font();
    }

    lf_next_line();

    {

        lf_push_font(&smaller_musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.margin_left = 15;
        props.text_color = (LfColor){148, 148, 148 , 255};
        lf_push_style_props(props);
        //lf_text_wide("music path :");
        lf_text("Genre :");
        lf_pop_style_props();
        lf_pop_font();

    }

    lf_next_line();

    static int selected_genre = -1;

    static const char* items[] = {"music" , "pop" , "hip-hop" , "k-pop" , "rock" , "chill" , "country" , "metal"}; 

//    if(false)
    {
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = (LfColor){18 , 18 , 18 , 255};
        //props.margin_left = 15;
        props.border_width = 0;
        props.text_color = LF_WHITE;
        //props.corner_radius = 0;
        props.border_color = (LfColor){180 , 180 , 180 , 255};
        lf_push_font(&musicNameFont);
        lf_push_style_props(props);
        lf_dropdown_menu(items , "Genre" , 8 , 200 , 190 , &selected_genre , &is_open_genre);
        lf_pop_style_props();
        lf_pop_font();
    }

    {
        bool form_complete = (strlen(input_str_artist) && strlen(input_str_path) && strlen(input_str_title) && selected_genre != -1);
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = (form_complete) ? (LfColor){65 , 167 , 204 , 255} : (LfColor){80 , 80 , 80 , 255};
        props.margin_left = 0;
        props.margin_right = 0;
        props.border_width = 2;
        props.text_color = LF_BLACK;
        props.corner_radius = 5;
        lf_push_style_props(props);
        //lf_set_line_should_overflow(false);
        
        lf_set_ptr_x_absolute(width - 200 - MARGIN - 5);
        lf_set_ptr_y_absolute(hight - 50 - MARGIN - 5);

        if(lf_button_fixed("Add Task" , 150 , -1) == LF_CLICKED && form_complete)
        {

            std::filesystem::path somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH = input_str_path;

            std::filesystem::path RM = "musics/";

            bool exist = false;
            int num = -1;
            
            if(std::filesystem::exists(RM / somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename()) == false)
                std::filesystem::copy(input_str_path , RM / somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename());
            else
            {
                return;
            }
            


            std::string cheese_berger = "musics/" + somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename().string();

            Song new_song(cheese_berger,std::string(input_str_title),std::string(input_str_artist),(Genre)selected_genre);

            new_song.changeArtistsName(input_str_artist);
            new_song.changeTitlesName(input_str_title);
            new_song.changeGenre(items[selected_genre]);

            playlist->addSong(new_song);

            current_tab = MAIN_PAGE;
            selected_genre = -1;
            memset(input_str_title, 0, sizeof(input_str_title));
            memset(input_str_path, 0, sizeof(input_str_path));
            memset(input_str_artist, 0, sizeof(input_str_artist));
            new_task_input_artist.cursor_index = 0;
            new_task_input_path.cursor_index = 0;
            new_task_input_title.cursor_index = 0;
            lf_input_field_unselect_all(&new_task_input_artist);
            lf_input_field_unselect_all(&new_task_input_path);
            lf_input_field_unselect_all(&new_task_input_title);
        }

        //lf_set_line_should_overflow(true);
        
        lf_pop_style_props();
        lf_pop_font();
    }

    {
        LfUIElementProps props = lf_get_theme().image_props;
        props.color = (LfColor){20,20,20,255};
        lf_push_style_props(props);
        lf_set_ptr_x_absolute(MARGIN + 14);
        lf_set_ptr_y_absolute(hight - MARGIN - 60);
        if (lf_image_button(((LfTexture){.id = backIcon.id , 28 , 32})) == LF_CLICKED)
        {
            current_tab = MAIN_PAGE;
        }
        lf_pop_style_props();
    }

    lf_div_end();

}

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

int main()
{

    initializeSDL();

    playlist = new DoubleLinkedList();


    srand(time(NULL));

    readFile();

    glfwInit();

    window = glfwCreateWindow(width , hight , "ff" , NULL , NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(width , hight , window);

    LfTheme theme = lf_get_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    InItIconsAndFonts();

    while (glfwWindowShouldClose(window) == false)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.07,0.07,0.07,0.07);

        lf_begin();

        switch (current_tab)
        {
        case MAIN_PAGE:
            RenderMainPage();
            break;
        case ADD_NEW_SONG:
            RenderAddNewSongPage();
            break;
        default:
            lf_text("something went wrong :(( ");
            break;
        }

        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    free_memory();
    if(Mix_Playing(-1) != 0)
        Mix_FreeMusic(sound);
    Mix_CloseAudio();
    SDL_Quit();
    

    return 0;
}
