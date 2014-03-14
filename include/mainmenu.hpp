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
#include"buttonobject.hpp"
#include<memory>

class MainMenuObject: public Scene {
public:
    MainMenuObject(SDLContext& context);
    ~MainMenuObject();

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
    std::unique_ptr<ButtonObject> playButton;
    std::unique_ptr<ButtonObject> quitButton;
};

#endif	/* MAINMENU_HPP */

