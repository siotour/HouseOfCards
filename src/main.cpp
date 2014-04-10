#include"../include/sdlutility.hpp"
#include"../include/scene.hpp"
#include"../include/mainmenu.hpp"
#include"../include/battlescene.hpp"
#include<iostream>
#include<memory>
#include<SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv)
{
    SDLContext sdlContext("House of Cards", 1024, 768, false);
    
    unique_ptr<Scene> scene;
    
    scene.reset(new MainMenu(sdlContext));
    
    bool quit = false;
    SDL_Event event;
    while(quit == false) {
        while(SDL_PollEvent(&event)) {
            if(scene->isDone() == true) {
                switch(scene->getNextSceneType()) {
                    case ST_Quit:
                        quit = true;
                        break;
                    case ST_Game:
                        scene.reset(new BattleScene(sdlContext));
                        break;
                    case ST_MainMenu:
                        scene.reset(new MainMenu(sdlContext));
                        break;
                }
            } else {
                scene->handleEvent(event);
                scene->update(0);
                SDL_RenderClear(sdlContext.getRenderer());
                scene->render(sdlContext);
            }
        }
        SDL_RenderPresent(sdlContext.getRenderer());
    }
    
    return 0;
}

