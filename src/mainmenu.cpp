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

const string PlayButtonFile = "assets/data/startButton.xml";
const string QuitButtonFile = "assets/data/mainQuitButton.xml";

const string MusicPath = "assets/audio/Track 12 - Music Box.ogg";

TextureID BackgroundTexID;
MusicID musicID;

const AABB2<double> BackgroundPos = {0, 0, 1, 1};
const AABB2<double> PlayButtonPos = {0.1, 0.3, 0.2074, 0.3521};
const AABB2<double> QuitButtonPos = {0.1, 0.4, 0.2074, 0.4521};
} // Anonymous namespace

MainMenu::MainMenu(SDLContext& context)
: sceneIsDone(false)
{
    try {
        SDLTextureManager& textureManager = context.getTextureManager();
        
        BackgroundTexID = textureManager.load(BackgroundImage);
        
        background = textureManager.getByID(BackgroundTexID);
        
        playButton.reset(fromXML(context, PlayButtonFile));
        playButton->setPosition(PlayButtonPos);
        quitButton.reset(fromXML(context, QuitButtonFile));
        quitButton->setPosition(QuitButtonPos);
        
        
        SDLMusicManager& musicManager = context.getMusicManager();
        musicID = musicManager.load(MusicPath);
        music = musicManager.getByID(musicID);
        
        SpriteLoader spriteLoader(textureManager);
        sprite.reset(new Sprite(spriteLoader.load("assets/data/testSprite.xml")));
        sprite->setDepth(0.5);
        sprite->setPosition({0, 0, 0.1, 0.07});
        sprite->setIdleAnimation(0);
        
        playMusic(music);
    } catch(...) {
        throw;
    }
}

MainMenu::~MainMenu() {
//    stopMusic();
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