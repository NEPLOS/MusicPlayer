
#pragma once

#include <iostream>
#include "linked_list.h"
#include "Song.h"
#include <filesystem>

#include "gtk-3.0/gtk/gtk.h"
#include "libayatana-appindicator3-0.1/libayatana-appindicator/app-indicator.h"

extern "C" {
    #include "leif/leif.h"
    #include <GLFW/glfw3.h>
    #include "tinyfiledialogs.h"
}

#define MARGIN 20

#define ONE_MUSIC 0
#define ALL_MUSICS 1

std::string fullHomePath = "/home/user/projects/MusicPlayer/";

enum Tab
{
    NONE = 0 ,
    MAIN_PAGE = 1 ,
    ADD_NEW_SONG = 2 ,
    EDIT_PAGE = 3

};

enum LoopMode
{
    LOOP_ALL,
    LOOP_ONE
};

GtkWidget* loop_all_item;
GtkWidget* loop_one_item;

Genre Filter = All;

Tab current_tab = MAIN_PAGE;

static bool loop_type = ONE_MUSIC;

static bool should_i_pause_or_unpause_the_music = true;
static bool should_i_mute_or_unmute_the_music = true;

enum Genre;

GLFWwindow* window;

bool canIjoin = true;

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

static int selected_genre = -1;

static bool renderUiEmergency = false;

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