#include"../include/mainmenu.hpp"
#include"../include/sdlutility.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include<memory>

using namespace std;


namespace {
const string BackgroundImage = "assets/textures/mainBackground.png";
const string PlayInactiveImage = "assets/textures/mainPlayInactive.png";
const string PlayActiveImage = "assets/textures/mainPlayActive.png";
const string QuitInactiveImage = "assets/textures/mainQuitInactive.png";
const string QuitActiveImage = "assets/textures/mainQuitActive.png";

SDLTextureManager::ID BackgroundTexID;
SDLTextureManager::ID PlayInactiveTexID;
SDLTextureManager::ID PlayActiveTexID;
SDLTextureManager::ID QuitInactiveTexID;
SDLTextureManager::ID QuitActiveTexID;

const SDL_Rect BackgroundPos = {0, 0, 1024, 768};
const SDL_Rect PlayButtonPos = {100, 210, 110, 40};
const SDL_Rect QuitButtonPos = {100, 330, 110, 40};
} // Anonymous namespace

MainMenu::MainMenu(SDLContext& context)
: sceneIsDone(false), textureManager(new SDLTextureLoader(context))
{
    try {
        BackgroundTexID = textureManager.load(BackgroundImage);
        PlayInactiveTexID = textureManager.load(PlayInactiveImage);
        PlayActiveTexID = textureManager.load(PlayActiveImage);
        QuitInactiveTexID = textureManager.load(QuitInactiveImage);
        QuitActiveTexID = textureManager.load(QuitActiveImage);
        
        background = textureManager.getByID(BackgroundTexID);
        
        SDL_Texture* playInactive = textureManager.getByID(PlayInactiveTexID);
        SDL_Texture* playActive = textureManager.getByID(PlayActiveTexID);
        playButton.reset(new Button(PlayButtonPos, playInactive, playActive));
        
        SDL_Texture* quitInactive = textureManager.getByID(QuitInactiveTexID);
        SDL_Texture* quitActive = textureManager.getByID(QuitActiveTexID);
        quitButton.reset(new Button(QuitButtonPos, quitInactive, quitActive));
    } catch(...) {
        throw;
    }
}

void MainMenu::update(const double deltaTime) {
    
}

void MainMenu::render(SDLContext& context) {
    SDL_RenderCopy(context.getRenderer(), background, NULL, &BackgroundPos);
    playButton->render(context);
    quitButton->render(context);
}

bool MainMenu::handleEvent(const SDL_Event& event) {
    bool eventHandled = false;
    
    if(event.type == SDL_QUIT) {
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

bool MainMenu::isDone() {
    return sceneIsDone;
}

SceneType MainMenu::getNextSceneType() {
    return nextScene;
}