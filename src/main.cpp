#include"../include/sdlutility.hpp"
#include"../include/scene.hpp"
#include"../include/mainmenu.hpp"
#include"../include/battlescene.hpp"
#include"../include/xmlutility.hpp"
#include<avl/include/exceptions.hpp>
#include<iostream>
#include<memory>
#include<string>
#include<SDL2/SDL.h>

using namespace std;
using namespace avl;

const string SettingsFile = "assets/data/settings.xml";

int runGame();

int main(int argc, char** argv)
{
//    
//    SDLContext context("House of Cards", 1024, 768, false);
//    SDLTextureManager textureManager(move(unique_ptr<SDLTextureLoader>(new SDLTextureLoader(context))));
    
    
//    
//    return 0;
    
    
    
    return runGame();
}


int runGame() {
    try {
        unique_ptr<SDLContext> context;
        context.reset(loadSettings(SettingsFile));
//        SDLContext sdlContext("House of Cards", 1024, 768, true);

        unique_ptr<Scene> scene;

        scene.reset(new MainMenu(*context));


        bool quit = false;
        SDL_Event sdlEvent;
        Event event;
        while(quit == false) {
            while(SDL_PollEvent(&sdlEvent)) {
                if(scene->isDone() == true) {
                    switch(scene->getNextSceneType()) {
                        case ST_Quit:
                            quit = true;
                            break;
                        case ST_Game:
                            scene.reset(new BattleScene(*context));
                            break;
                        case ST_MainMenu:
                            scene.reset(new MainMenu(*context));
                            break;
                    }
                } else {
                    if(makeEvent(*context, sdlEvent, event) == true) {
                        scene->handleEvent(event);
                        scene->update(0);
                    }
                        SDL_RenderClear(context->getRenderer());
                        scene->render(*context);
                }
            }
            context->present();
        }
    } catch(const Exception& e) {
        cout << "avl::Exception thrown: " << e.getMessage() << endl;
    }
    
    
    return 0;
}