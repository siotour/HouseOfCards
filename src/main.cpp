#include"../include/sdlutility.hpp"
#include"../include/mainmenu.hpp"
#include<iostream>
#include<SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv)
{
    SDLContext sdlContext("House of Cards", 1024, 768, false);
    
    MainMenuObject menu(sdlContext);
    
    bool quit = false;
    SDL_Event event;
    while(quit == false) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else {
                menu.handleEvent(event);
                menu.update(0);
                SDL_RenderClear(sdlContext.getRenderer());
                menu.render(sdlContext);
            }
        }
        SDL_RenderPresent(sdlContext.getRenderer());
    }
    
    return 0;
}

