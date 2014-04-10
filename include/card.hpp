/* 
 * File:   card.hpp
 * Author: sbachstein
 *
 * Created on March 14, 2014, 8:24 AM
 */

#ifndef CARD_HPP
#define	CARD_HPP

#include"object.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<vector>
#include<memory>


class Card: public Object {
public:
    Card();
    virtual ~Card();

    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
    void setPosition(avl::Vec2<unsigned short> newPosition);
    
private:
    void startDrag();
    void stopDrag();
    void showPreview();
    void hidePreview();
    void cleanup();
    
    bool isBeingDragged;
    bool showPreview;
    avl::Vec2<unsigned short> thumbnailPosition;
    
    
    // Small version of card shown in hand
    SDL_Texture* cardThumbnail;
    // Magnified version of card shown during mouse-over
    SDL_Texture* cardPreview;
    
    
};


#endif	/* CARD_HPP */

