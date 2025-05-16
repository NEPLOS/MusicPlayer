
#pragma once

#include "stuff.h"

void RenderAddNewSongPage()
{
    lf_div_begin(((vec2s){MARGIN , MARGIN}) , ((vec2s){ width - MARGIN * 2 ,hight - MARGIN * 2}) , true );

    // set the theme
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

        lf_push_style_props(props);

        if (lf_image_button(((LfTexture){.id = searchIcon.id , 24 , 24})) == LF_CLICKED)
        {
            const char* filters[1] = {"*.mp3"};

            // something i don't really understand , but it works , according to the documents this is the right way to do this (thanks to the tinyfiledialogs dude ) 
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
        lf_text("Genre :");
        lf_pop_style_props();
        lf_pop_font();

    }

    lf_next_line();

    static int selected_genre = -1;

    static const char* items[] = {"music" , "pop" , "hip-hop" , "k-pop" , "rock" , "chill" , "country" , "metal"}; // selection box (combo box) for genre

    {
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = (LfColor){18 , 18 , 18 , 255};
        props.border_width = 0;
        props.text_color = LF_WHITE;
        props.border_color = (LfColor){180 , 180 , 180 , 255};
        lf_push_font(&musicNameFont);
        lf_push_style_props(props);
        lf_dropdown_menu(items , "Genre" , 8 , 200 , 190 , &selected_genre , &is_open_genre);
        lf_pop_style_props();
        lf_pop_font();
    }

    if(show_warning)
    {
        lf_set_ptr_x_absolute(450);
        lf_set_ptr_y_absolute(500);
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().text_props;
        props.color = (LfColor){255 , 15 , 15 , 200};
        props.margin_bottom = 20;
        lf_push_style_props(props);
        lf_text("music already exist");
        lf_pop_style_props();
        lf_pop_font();
    }

    // add new Music logic here
    {
        bool form_complete = (strlen(input_str_artist) && strlen(input_str_path) && strlen(input_str_title) && selected_genre != -1); // check if the form is completed or not
        lf_push_font(&musicNameFont);
        LfUIElementProps props = lf_get_theme().button_props;
        props.color = (form_complete) ? (LfColor){65 , 167 , 204 , 255} : (LfColor){80 , 80 , 80 , 255};
        props.margin_left = 0;
        props.margin_right = 0;
        props.border_width = 2;
        props.text_color = LF_BLACK;
        props.corner_radius = 5;
        lf_push_style_props(props);
        
        lf_set_ptr_x_absolute(width - 200 - MARGIN - 5);
        lf_set_ptr_y_absolute(hight - 50 - MARGIN - 5);

        if(lf_button_fixed("Add Task" , 150 , -1) == LF_CLICKED && form_complete) // if the form if complete
        {

            std::filesystem::path somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH = input_str_path; // get the new music path

            std::filesystem::path RM = fullHomePath + "musics/"; // music folder

            bool exist = false;
            int num = -1;
            
            if(std::filesystem::exists(RM / somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename()) == false)
                std::filesystem::copy(input_str_path , RM / somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename()); // basic validations
            else
            {
                std::cout << "file already exist\n";
                show_warning = true;
                return;
            }


            std::string cheese_berger = fullHomePath + "musics/" + somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename().string(); // full path

            Song new_song(cheese_berger,std::string(input_str_title),std::string(input_str_artist),(Genre)selected_genre); // create the song

            new_song.changeArtistsName(input_str_artist);  // change the music artists 
            new_song.changeTitlesName(input_str_title);    // change the music title
            new_song.changeGenre(items[selected_genre]);   // change the music genre

            if(!(playlist->addSong(new_song))) // add the song to the playlist
            {
                std::cerr << "failed to add music\n";
                show_warning = true;
                std::filesystem::remove(RM / somewhere_idk_im_losing_my_sanity_its_2AM_AHHHHHHH.filename());
                return;
            }

            if(should_i_sort)
            {
                playlist->Sort((SORT_FILTER)(!current_sort)); // resort the playlist
            }

            // free memory , setting stuff
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

            current_tab = MAIN_PAGE; // set the page to the main page
            show_warning = false;
        
        }
        
        lf_pop_style_props();
        lf_pop_font();
    }

    // the back button
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