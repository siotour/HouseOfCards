/* 
 * File:   mainmenu.hpp
 * Author: sbachstein
 *
 * Created on March 12, 2014, 12:33 PM
 */

#ifndef MAINMENU_HPP
#define	MAINMENU_HPP

#include"sdlutility.hpp"
#include"scene.hpp"
#include"base.hpp"
#include"button.hpp"
#include"sprite.hpp"
#include<memory>
// Temporary
#include<SDL2/SDL_mixer.h>

class MainMenu: public Scene {
public:
    MainMenu(SDLContext& context);
    ~MainMenu() = default;

    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const Event& event);
    
    bool isDone();
    SceneType getNextSceneType();
    
private:
    void cleanup();
    
    bool sceneIsDone;
    SceneType nextScene;
    
    std::unique_ptr<Sprite> sprite;
    
    SDL_Texture* background;
    std::unique_ptr<Button> playButton;
    std::unique_ptr<Button> quitButton;
    
    SoundID selectSound;
    // Temporary
    Mix_Chunk* selectSoundPtr;
    MusicID music;
};

#endif	/* MAINMENU_HPP */

