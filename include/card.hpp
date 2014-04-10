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
    // Ownership of thumbnail and preview are left with the caller. These pointers
    // must remain valid throughout the lifetime of this object.
    Card(avl::Vec2<short> position, SDL_Texture* const thumbnail, SDL_Texture* const preview);
    virtual ~Card();

    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
    void setPosition(avl::Vec2<short> newPosition);
    
private:
    void startDrag();
    void stopDrag();
    void showPreview();
    void hidePreview();
    bool handleMouseMove(const SDL_MouseMotionEvent motion);
    bool handleMouseButton(const SDL_MouseButtonEvent button);
    bool handleKey(const SDL_KeyboardEvent key);
    void cleanup();
    
    avl::Vec2<short> thumbnailPosition;
    bool isBeingDragged;
    bool previewOn;
    
    
    // Small version of card shown in hand
    SDL_Texture* cardThumbnail;
    // Magnified version of card shown during mouse-over
    SDL_Texture* cardPreview;
    
    
};


#endif	/* CARD_HPP */

