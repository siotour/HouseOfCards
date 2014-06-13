#include"../include/sdlutility.hpp"
#include"../include/scene.hpp"
#include"../include/mainmenu.hpp"
#include"../include/battlescene.hpp"
#include"../include/xmlutility.hpp"
// For testing
#include"../include/pathfinding.hpp"
// For testing
#include<avl/include/utility.hpp>
#include<avl/include/exceptions.hpp>
#include<avl/include/timer.hpp>
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
//    SDLContext context("House of Cards", 1024, 768, false);
//    SDLTextureManager textureManager(move(unique_ptr<SDLTextureLoader>(new SDLTextureLoader(context))));
//    Network<4, 4> network;
//    for(int x = 0; x < 4; ++x) {
//        for(int y = 0; y < 4; ++y) {
//            network.nodes[x][y] = false;
//        }
//    }
//    
////    network.nodes[0][3] = true;
//    network.nodes[0][3] = true;
//    
//    
//    RoomCoord start{1, 3};
//    RoomCoord end{0, 3};
//    
//    Path path = breadthFirstSearch(start, end, network);
//    
//    if(path.empty() == true) {
//        cout << "Path doesn't exist!" << endl;
//    }
//    for(auto vertex : path) {
//        cout << "(" << vertex.x << ", " << vertex.y << ")" << endl;
//    }
//    
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
        Timer timer;
        while(quit == false) {
            scene->update(timer.reset());
            SDL_RenderClear(context->getRenderer());
            scene->render(*context);
            
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
                    }
                }
            }
            
            context->present();
        }
    } catch(const Exception& e) {
        cout << "avl::Exception thrown: " << e.getMessage() << endl;
    }
    
    
    return 0;
}