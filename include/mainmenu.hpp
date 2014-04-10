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
#include"button.hpp"
#include<memory>

const SceneType ST_Quit = 0;
const SceneType ST_Game = 1;

class MainMenu: public Scene {
public:
    MainMenu(SDLContext& context);
    ~MainMenu();

    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
    bool isDone();
    SceneType getNextSceneType();
    
private:
    void cleanup();
    
    bool sceneIsDone;
    SceneType nextScene;
    
    SDL_Texture* background;
    std::unique_ptr<Button> playButton;
    std::unique_ptr<Button> quitButton;
};

#endif	/* MAINMENU_HPP */

