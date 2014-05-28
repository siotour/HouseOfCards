#include"../include/mainmenu.hpp"
#include"../include/sdlutility.hpp"
#include"../include/xmlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include<memory>

using namespace std;
using namespace avl;


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

const AABB2<double> BackgroundPos = {0, 0, 1, 1};
const AABB2<double> PlayButtonPos = {0.1, 0.3, 0.2074, 0.3521};
const AABB2<double> QuitButtonPos = {0.1, 0.4, 0.2074, 0.4521};
} // Anonymous namespace

MainMenu::MainMenu(SDLContext& context)
: sceneIsDone(false), textureManager(move(unique_ptr<SDLTextureLoader>(new SDLTextureLoader(context))))
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
        
        SpriteLoader spriteLoader(textureManager);
        sprite.reset(new Sprite(spriteLoader.load("assets/data/testSprite.xml")));
        sprite->setDepth(0.5);
        sprite->setPosition({0, 0, 0.1, 0.07});
        sprite->setIdleAnimation(0);
    } catch(...) {
        throw;
    }
}

void MainMenu::update(const double deltaTime) {
    sprite->update(deltaTime);
}

void MainMenu::render(SDLContext& context) {
    context.renderTexture(background, NULL, &BackgroundPos);
    playButton->render(context);
    quitButton->render(context);
    sprite->render(context);
    
}

bool MainMenu::handleEvent(const Event& event) {
    bool eventHandled = false;
    
    if(event.type == ET_Quit) {
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