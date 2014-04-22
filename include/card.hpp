/* 
 * File:   card.hpp
 * Author: sbachstein
 *
 * Created on March 14, 2014, 8:24 AM
 */

#ifndef CARD_HPP
#define	CARD_HPP

#include"object.hpp"
#include"fort.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<vector>
#include<memory>


typedef unsigned short CardID;

enum CardType {
    CT_Unknown,
    CT_Room
};


class Card: public Object {
public:
    // Ownership of thumbnail and preview are left with the caller. These pointers
    // must remain valid throughout the lifetime of this object.
    Card(avl::Vec2<short> position, SDL_Texture* const thumbnail, SDL_Texture* const preview);
    virtual ~Card();

    virtual void update(const double deltaTime);

    virtual void render(SDLContext& context);
    virtual bool handleEvent(const SDL_Event& event);
    
    void setPosition(avl::Vec2<short> newPosition);
    
protected:
    virtual void startDrag();
    virtual void stopDrag();
    void showPreview();
    void hidePreview();
    virtual bool handleMouseMove(const SDL_MouseMotionEvent motion);
    virtual bool handleMouseButton(const SDL_MouseButtonEvent button);
    virtual bool handleKey(const SDL_KeyboardEvent key);
    virtual void cleanup();
    
    CardType type;
    CardID id;
    
    avl::Vec2<short> thumbnailPosition;
    bool isBeingDragged;
    bool previewOn;
    
    
    // Small version of card shown in hand
    SDL_Texture* cardThumbnail;
    // Magnified version of card shown during mouse-over
    SDL_Texture* cardPreview;
    
};


class RoomCard: public Card {
public:
    RoomCard(avl::Vec2<short> position, SDL_Texture* const thumbnail, SDL_Texture* const preview, Fort& fort);
    ~RoomCard() = default;
    
private:
    void startDrag();
    void stopDrag();
    
    bool handleMouseMove(const SDL_MouseMotionEvent motion);
    
    LocationMap potentialLocations;
    
    Fort& fort;
};


#endif	/* CARD_HPP */

