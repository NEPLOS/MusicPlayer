
#include "gtk-3.0/gtk/gtk.h"
#include "libayatana-appindicator3-0.1/libayatana-appindicator/app-indicator.h"

#include "stuff.h"
#include "load_and_free.h"

#include "mainpage.h"
#include "addSongPage.h"

#include <thread>
#include <atomic>

#include <csignal>

std::atomic<bool> guiRunning(false);
std::thread guiThread;

std::atomic<bool> running(true);
std::thread musicWatcherThread;

void handle_exit_signal(int signum) 
{
    running = false;
    gtk_main_quit();
}

void RenderGui()
{

    guiRunning = true;
    glfwInit();

    window = glfwCreateWindow(width , hight , "ff" , NULL , NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    lf_init_glfw(width , hight , window);

    LfTheme theme = lf_get_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    InItIconsAndFonts();

    while (glfwWindowShouldClose(window) == false)
    {
        if (!glfwGetWindowAttrib(window, GLFW_ICONIFIED) && glfwGetWindowAttrib(window, GLFW_FOCUSED)) 
        {

            glfwPollEvents();
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
                // for unexpected behaviour 
                lf_text("something went wrong :(( ");
                break;
            }

            lf_end();

            glfwPollEvents();
            glfwSwapBuffers(window);
        } 
        else 
        {

            SDL_Delay(100);
            continue;
        }

    }
    glfwDestroyWindow(window);
    guiRunning = false;
}

void repeat_music() 
{

    if (loop_type == ONE_MUSIC)//(Mix_PlayingMusic() == 0) && (playing_song != nullptr) && (sound != nullptr) && Mix_Playing(-1) == 0) 
    {
        //SDL_Delay(200);
        playing_song->song.loadAndplay();
    }
}


static void on_show_gui(GtkMenuItem*, gpointer)
{
    // render gui
    if (!guiRunning)
    {
        guiThread = std::thread(RenderGui);
        guiThread.detach();
    }
}

// close the program
static void on_quit(GtkMenuItem*, gpointer)
{
    free_memory();
    if(Mix_Playing(-1) != 0)
    Mix_FreeMusic(sound);
    Mix_CloseAudio();
    SDL_Quit();
    gtk_main_quit();
}

// if music is playing pause or resume it
static void on_pause_and_resume(GtkMenuItem*, gpointer)
{
    if(playing_song != nullptr)
    {
        playing_song->song.pauseMusic();
    }
}


int main()
{

    initializeSDL();

    playlist = new DoubleLinkedList();

    srand(time(NULL));

    readFile();

    gtk_init(nullptr, nullptr);

    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);

    AppIndicator* indicator = app_indicator_new(
        "ff-tray",
        "dialog-information",
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    GtkWidget* menu = gtk_menu_new();
    GtkWidget* show_item = gtk_menu_item_new_with_label("Open ff");
    GtkWidget* music_item = gtk_menu_item_new_with_label("pause/resume music");
    GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");

    g_signal_connect(show_item, "activate", G_CALLBACK(on_show_gui), nullptr);
    g_signal_connect(music_item, "activate", G_CALLBACK(on_pause_and_resume), nullptr);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit), nullptr);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), show_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), music_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);
    gtk_widget_show_all(menu);

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    //musicWatcherThread = std::thread(MusicWatcherLoop);

    Mix_HookMusicFinished(repeat_music);

    // reender GUI
    guiThread = std::thread(RenderGui);
    guiThread.detach();

    gtk_main();

    running = false;
    if (musicWatcherThread.joinable())
        musicWatcherThread.join();

    // exit and free
    free_memory();
    if(Mix_Playing(-1) != 0)
        Mix_FreeMusic(sound);
    Mix_CloseAudio();
    SDL_Quit();
    

    return 0;
}