
#pragma once

#include "stuff.h"

// renders the top UI
void TopUI()
{
    lf_push_font(&titleFont);
    lf_text("Music Program");
    lf_pop_font();

    // title search UI
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

    // add new music UI
    {
        lf_set_ptr_x_absolute(width - 160 - MARGIN * 2);
        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = 0;
        props.margin_right = 0;
        props.color = (LfColor){65, 167, 204 ,255};
        props.border_width = 0;
        props.corner_radius = 4;
        lf_push_style_props(props);
        if(lf_button_fixed("Add New Music" , 160 , -1) == LF_CLICKED)
        {
            current_tab = ADD_NEW_SONG;
        }

        lf_pop_style_props();

    }

    lf_next_line();

    // filter by genre UI
    {
        static int selected_genre = 0;

        static const char* items[] = { "All" , "music" , "pop" , "hip-hop" , "k-pop" , "rock" , "chill" , "country" , "metal"}; 

        {
            LfUIElementProps props = lf_get_theme().button_props;
            props.color = (LfColor){18 , 18 , 18 , 255};
            props.border_width = 0;
            props.text_color = LF_WHITE;
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

    }

    // search by artist UI
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

    // delete music icon (based on artist . . . will add by genre later)
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
                playlist->removeSong_by_artist_genre_search(input_str_artist_search , Filter);

                if (playing_song != nullptr)
                {
                    playing_song = playing_song->next;
                    Mix_FreeMusic(sound);

                    if (playing_song != nullptr)
                    {
                        playing_song->song.loadAndplay();
                        space_key = true;
                    }
                }
            }
            memset(input_str_artist_search, 0, sizeof(input_str_artist_search));
            
        }

        lf_pop_style_props();

    }


    // sort base on artist or name UI
    {
        lf_set_ptr_x_absolute(width - 160 - MARGIN * 2);
        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = -8;
        props.margin_top = 7;
        props.color = (LfColor){65, 167, 204 ,255};
        props.border_width = 0;
        props.corner_radius = 4;
        lf_push_style_props(props);

        std::string name = (current_sort)? "Artist" : "Title";

        name = "sort base on " + name;

        if(lf_button_fixed(name.c_str() , 176 , -1) == LF_CLICKED)
        {
            current_sort = !current_sort;

            (current_sort)? playlist->Sort(MUSIC_TITLE) : playlist->Sort(MUSIC_ARTIST);            

        }

        lf_pop_style_props();
    }

    // the song name UI
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

// side panel , render
void sidePanel()
{

    if (playlist->head == nullptr) // if it's null . . . no music :((
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

    while (current)
    {

        if (Filter != All && current->song.genre != Filter)
        {
            current = current->next;
            continue;
        }

        // search base on title Logic
        {
            std::string title_song = current->song.song_name;
            std::string input_title_song = input_str_title_search;
            
            title_song = UpperCase(title_song);
            input_title_song = UpperCase(input_title_song);


            if (!input_title_song.empty() && title_song.find(input_title_song) == std::string::npos)
            {
                current = current->next;
                continue;
            }
            
        }

        // search base on artist Logic
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

        // the play button logic and UI
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = LF_NO_COLOR;
        props.border_width = 0;
        props.margin_right = 10;
        props.margin_bottom = 10;

        lf_push_style_props(props);

        if (current->song.paused == true)
        {

            if ((Mix_PlayingMusic() == 0) && (playing_song != nullptr) && (sound != nullptr) && Mix_Playing(-1) == 0) // if no song is playing
            {
                if (loop_type) // if loop type is all musics . . . it will load the next music in the playlist (if it exists)
                {
                    if(playing_song->next != nullptr)
                    {
                        if(!(playing_song->song.paused)) playing_song->song.paused = true;
                        playing_song = playing_song->next;
                        playing_song->song.loadAndplay();
                    }
                    else
                    {
                        if(!(playing_song->song.paused)) playing_song->song.paused = true;
                        playing_song = nullptr;
                        glfwSetWindowTitle(window , "ff");
                        Mix_FreeMusic(sound);
                    }
                }
                else // else loop the current music
                {
                    playing_song->song.loadAndplay();
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
                    playing_song->song.loadAndplay();
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

        // render the music name
        float current_x = lf_get_ptr_x();
        {
            lf_set_line_should_overflow(false);

            lf_set_ptr_y_absolute(lf_get_ptr_y() + 7);
            lf_push_font(&musicNameFont);
            std::string nero = current->song.song_name;

            if (nero.size() > 15) {
                nero = nero.substr(0, 15 - 3) + "...";
            }

            lf_set_text_wrap(true);
            lf_text(nero.c_str());
            lf_set_text_wrap(false);

            lf_set_ptr_y_absolute(lf_get_ptr_y() + 25);
            lf_set_ptr_x_absolute(current_x + 3);
            
            lf_pop_font();
            lf_set_line_should_overflow(true);
        }

        // remove music logic
        {
            LfUIElementProps props = lf_get_theme().button_props;
            props.margin_left = 3;
            props.color = LF_NO_COLOR;
            props.border_width = 0;

            lf_set_ptr_y_absolute(lf_get_ptr_y() - 32);
            lf_set_ptr_x_absolute(current_x + 150);

            lf_push_style_props(props);

            if (lf_image_button(((LfTexture){.id = trashbinIcon.id , 24 , 24})) == LF_CLICKED)
            {   
                // i can change it later yeah sure , but for now , it works
                playlist->removeSonge_by_name(current->song.song_name);

                if (playing_song == current)
                {
                    playing_song = playing_song->next;
                    Mix_FreeMusic(sound);

                    if (playing_song != nullptr)
                    {
                        playing_song->song.loadAndplay();
                        space_key = true;
                    }
                }
                
                
            }

            lf_pop_style_props();

        }

        lf_set_ptr_y_absolute(lf_get_ptr_y() + 30);
        lf_set_ptr_x_absolute(current_x + 2);

        // render the music artist
        {

            lf_push_font(&smaller_musicNameFont);
            lf_text(current->song.artist.c_str());
            lf_pop_font();

            lf_next_line();
            current = current->next;
        }


        count++;
    }

    // if no music found in the search logic
    if(count == 0)
    {
        lf_push_font(&musicNameFont);
        lf_text("no Music found :(( ");
        lf_pop_font();

        return;
    }

}

// render the bottom panel
void bottomPanel()
{
    if (playing_song == nullptr)
    {
        return;
    }

    bool key = lf_key_is_down(32);

    if (key && space_key && should_i_pause_or_unpause_the_music) // space button can pause and unpause the music
    {
        playing_song->song.pauseMusic();
        std::cerr << "you pressed space\n";
        space_key = false;
    }

    if (lf_key_is_released(32))
    {
        space_key = true;
    }

    int herbert = (Mix_GetMusicPosition(sound) * 100)/((float)(playing_song->song.time));

    herbert = herbert * 5;

    float current_x = lf_get_ptr_x();

    float slider_y = lf_get_ptr_y();

    // i tried to simulate an scroll bar (idk the name don't blame me) for changing the music posotion , it works but it's kinda buggy though
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

    // shuffle icon and logic (random)
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
            //playing_song->song.freeMusic();
            playing_song = temp;
            playing_song->song.loadAndplay();
            
            
        }


        lf_pop_style_props();
    }

    lf_set_ptr_y_absolute(slider_y + 16);

    // previous Icon
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
            Node* current = playing_song;
            current = current->pre;
            while (current != nullptr)
            {
                if (Filter == Genre::All)
                {
                    if (playing_song->pre != nullptr)
                    {
                        if(!playing_song->song.paused) playing_song->song.pauseMusic();
                        //playing_song->song.freeMusic();
                        playing_song = playing_song->pre;
                        playing_song->song.loadAndplay();
                    }
                    else
                    {
                        //playing_song->song.freeMusic();
                        playing_song->song.loadAndplay();
                    }
                    break;
                }
                else
                {
                    if (current->song.genre == Filter)
                    {
                        if(!playing_song->song.paused) playing_song->song.pauseMusic();
                        //playing_song->song.freeMusic();
                        playing_song = current;
                        playing_song->song.loadAndplay();
                        break;
                    }
                    
                }
                
                current = current->pre;
            }

            if (current == nullptr)
             {
                //playing_song->song.freeMusic();
                playing_song->song.loadAndplay();
            }
            
            
        }


        lf_pop_style_props();

    }

    lf_set_ptr_y_absolute(slider_y);
    
    // play and pause the music ICON
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

    // next music Icon
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

            Node* current = playing_song;
            current = current->next;
            while (current != nullptr)
            {
                if (Filter == Genre::All)
                {
                    if (playing_song->next != nullptr)
                    {
                        if(!playing_song->song.paused) playing_song->song.pauseMusic();
                        //playing_song->song.freeMusic();
                        playing_song = playing_song->next;
                        playing_song->song.loadAndplay();
                    }
                    else
                    {
                        //playing_song->song.freeMusic();
                        playing_song->song.loadAndplay();
                    }
                    break;
                }
                else
                {
                    if (current->song.genre == Filter)
                    {
                        if(!playing_song->song.paused) playing_song->song.pauseMusic();
                        //playing_song->song.freeMusic();
                        playing_song = current;
                        playing_song->song.loadAndplay();
                        break;
                    }
                    
                }
                
                current = current->next;
            }

            if (current == nullptr)
             {
                //playing_song->song.freeMusic();
                playing_song->song.loadAndplay();
            }
            
            
        }


        lf_pop_style_props();

    }

    lf_set_ptr_y_absolute(slider_y + 21);

    int temp_x = lf_get_ptr_x();

    // loop icon UI
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

        bool key = lf_key_went_down(77);

        if(key && should_i_mute_or_unmute_the_music) // M can also mute and unmute the music and it will remember the last music volume
        {
            std::cout << "you pressed M \n";
        }

        // render Icons
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

    lf_set_ptr_y_absolute(hight - MARGIN * 2 - 3);
    lf_set_ptr_x_absolute(lf_get_ptr_x() - 135);

    float current_x_2 = lf_get_ptr_x();
    

    float slider_y_2 = lf_get_ptr_y();

    // same scroll bar logic . . . different reason
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
