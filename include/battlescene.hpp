/* 
 * File:   battlescene.hpp
 * Author: sbachstein
 *
 * Created on April 10, 2014, 11:05 AM
 */

#ifndef BATTLESCENE_HPP
#define	BATTLESCENE_HPP

#include"scene.hpp"
#include"card.hpp"
#include"button.hpp"
#include"sdlutility.hpp"
#include"fort.hpp"
#include"cardfactory.hpp"
#include<memory>
#include<vector>


class BattleScene: public Scene {
public:
    BattleScene(SDLContext& context);
    ~BattleScene();
    
    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
    bool isDone();
    SceneType getNextSceneType();
    
private:
    void drawCard();
    void deleteCard(const unsigned int index);
    void quit();
    void loadTextures();
    void loadCards();
    void cleanup();
    
    bool sceneDone;
    SceneType nextScene;
    
    SDL_Texture* background;
    SDL_Texture* cardThumbnails[3];
    SDL_Texture* cardPreviews[3];
    std::vector<std::unique_ptr<Card>> cards;
    std::unique_ptr<Button> deckButton;
    std::unique_ptr<Button> quitButton;
    
    std::unique_ptr<Fort> fort;
    std::vector<CardID> cardIDs;
    CardFactory cardFactory;
    
    SDLTextureManager textureManager;
};


#endif	/* BATTLESCENE_HPP */
