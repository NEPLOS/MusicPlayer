
#include "stuff.h"
#include "load_and_free.h"

#include "mainpage.h"
#include "addSongPage.h"

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
            // for unexpected behaviour 
            lf_text("something went wrong :(( ");
            break;
        }

        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    // exit and free
    free_memory();
    if(Mix_Playing(-1) != 0)
        Mix_FreeMusic(sound);
    Mix_CloseAudio();
    SDL_Quit();
    

    return 0;
}
