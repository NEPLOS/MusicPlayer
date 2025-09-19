
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

// renders GUI
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

// ummmm yeah....
void repeat_music() 
{

    if (loop_type == ONE_MUSIC)//(Mix_PlayingMusic() == 0) && (playing_song != nullptr) && (sound != nullptr) && Mix_Playing(-1) == 0) 
    {
        //SDL_Delay(200);
        playing_song->song.loadAndplay();
    }
    else if (loop_type == ALL_MUSICS)
    {
        if(playing_song->next != nullptr)
        {
            playing_song = playing_song->next;
            playing_song->song.loadAndplay();
        }
        else
        {
            playing_song = playlist->head;
            playing_song->song.loadAndplay();
        }
    }
    
}

// renders gui on click
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

// change loop type
static void on_loop_mode_changed(GtkMenuItem*, gpointer loopType)
{
    LoopMode mode = (LoopMode)GPOINTER_TO_INT(loopType);

    if (mode == LOOP_ALL) 
    {
        loop_type = ALL_MUSICS;
    } 
    else if (mode == LOOP_ONE) 
    {
        loop_type = ONE_MUSIC;
    }
    
}


int main()
{

    initializeSDL();

    playlist = new DoubleLinkedList();

    srand(time(NULL));

    readFile();

    gtk_init(nullptr, nullptr);

    // using namespace std::literals;

    // // Execute lambda asyncronously.
    // auto f = std::async(std::launch::async, [] {
    //     auto s = ""s;
    //     if (std::cin >> s) return s;
    // });

    // // Continue execution in main thread.
    // while(f.wait_for(2s) != std::future_status::ready) {
    //     std::cout << "still waiting..." << std::endl;
    // }

    // std::cout << "Input was: " << f.get() << std::endl;

    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);

    AppIndicator* indicator = app_indicator_new(
        "ff-tray",
        "dialog-information",
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    GtkWidget* menu = gtk_menu_new();
    GtkWidget* show_item = gtk_menu_item_new_with_label("Open ff");
    GtkWidget* music_item = gtk_menu_item_new_with_label("Pause/Resume music");

    GtkWidget* change_loop_item = gtk_menu_item_new_with_label("Loop type");
    GtkWidget* change_loop_type_menu = gtk_menu_new();

    GtkWidget* loop_all_item = gtk_radio_menu_item_new_with_label(nullptr, "Loop All");
    GtkWidget* loop_one_item = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(loop_all_item), "Loop One");

    GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");

    g_signal_connect(show_item, "activate", G_CALLBACK(on_show_gui), nullptr);
    g_signal_connect(music_item, "activate", G_CALLBACK(on_pause_and_resume), nullptr);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit), nullptr);

    g_signal_connect(loop_all_item, "toggled", G_CALLBACK(on_loop_mode_changed), GINT_TO_POINTER(LOOP_ALL));
    g_signal_connect(loop_one_item, "toggled", G_CALLBACK(on_loop_mode_changed), GINT_TO_POINTER(LOOP_ONE));
    

    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(loop_one_item), TRUE);


    gtk_menu_shell_append(GTK_MENU_SHELL(change_loop_type_menu), loop_all_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(change_loop_type_menu), loop_one_item);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(change_loop_item), change_loop_type_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), show_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), music_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), change_loop_item);
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