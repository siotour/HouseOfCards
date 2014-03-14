#include"../include/mainmenu.hpp"
#include"../include/sdlutility.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include<memory>

using namespace std;


const string BackgroundImage = "assets/textures/mainBackground.png";
const string PlayInactiveImage = "assets/textures/mainPlayInactive.png";
const string PlayActiveImage = "assets/textures/mainPlayActive.png";
const string QuitInactiveImage = "assets/textures/mainQuitInactive.png";
const string QuitActiveImage = "assets/textures/mainQuitActive.png";

const SDL_Rect BackgroundPos = {0, 0, 1024, 768};
const SDL_Rect PlayButtonPos = {100, 210, 110, 40};
const SDL_Rect QuitButtonPos = {100, 330, 110, 40};


MainMenuObject::MainMenuObject(SDLContext& context) {
    try {
        background = IMG_LoadTexture(context.getRenderer(), BackgroundImage.c_str());
        if(background == nullptr) {
            cleanup();
            throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
        }
        
        playButton.reset(new ButtonObject(PlayButtonPos, context, PlayInactiveImage, PlayActiveImage));
        quitButton.reset(new ButtonObject(QuitButtonPos, context, QuitInactiveImage, QuitActiveImage));
    } catch(...) {
        cleanup();
        throw;
    }
}

MainMenuObject::~MainMenuObject() {
    cleanup();
}

void MainMenuObject::update(const double deltaTime) {
    
}

void MainMenuObject::render(SDLContext& context) {
    SDL_RenderCopy(context.getRenderer(), background, NULL, &BackgroundPos);
    playButton->render(context);
    quitButton->render(context);
}

bool MainMenuObject::handleEvent(const SDL_Event& event) {
    bool eventHandled = false;
    
    if(event.type == SDL_Quit) {
        sceneIsDone = true;
        nextScene = ST_Quit;
    }else if(playButton->handleEvent(event) == true) {
        eventHandled = true;
        sceneIsDone = true;
        nextScene = ST_Game;
    } else if(quitButton->handleEvent(event) == true) {
        eventHandled = true;
        sceneIsDone = true;
        nextScene = ST_Quit;
    }
    
    return eventHandled;
}

bool MainMenuObject::isDone() {
    return sceneIsDone;
}

void MainMenuObject::cleanup() {
    if(background != nullptr) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }
}