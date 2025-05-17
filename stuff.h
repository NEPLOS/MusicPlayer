
#pragma once

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

std::string fullHomePath = "";

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

Node* playing_song = nullptr;

static bool is_mute = false;
static int last_audio = MIX_MAX_VOLUME / 2;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

bool current_sort = true;

bool UP_ARROW = true;
bool DOWN_ARROW = true;
bool should_i_sort = false;
bool show_warning = false;