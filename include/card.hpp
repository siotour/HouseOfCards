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
    Card(const Card& original);
    virtual ~Card() = default;
    
    virtual Card* clone() const = 0;
    
    CardType getType() const;
    CardID getID() const;
    
    bool isDead() const;

    virtual void update(const double deltaTime);

    virtual void render(SDLContext& context);
    virtual bool handleEvent(const SDL_Event& event);
    
    void setPosition(avl::Vec2<short> newPosition);
    
protected:
    Card(const CardID id, SDL_Texture* const thumbnail, SDL_Texture* const preview);
    virtual void startDrag();
    virtual void stopDrag();
    void showPreview();
    void hidePreview();
    virtual bool handleMouseMove(const SDL_MouseMotionEvent motion);
    virtual bool handleMouseButton(const SDL_MouseButtonEvent button);
    virtual bool handleKey(const SDL_KeyboardEvent key);
    
    CardType type;
    CardID id;
    
    avl::Vec2<short> thumbnailPosition;
    bool dead;
    bool isBeingDragged;
    bool previewOn;
    
    
    // Small version of card shown in hand
    SDL_Texture* cardThumbnail;
    // Magnified version of card shown during mouse-over
    SDL_Texture* cardPreview;
    
};


class RoomCard: public Card {
public:
    RoomCard(const CardID id, SDL_Texture* const thumbnail, SDL_Texture* const preview, const Room& room, Fort& fort);
    RoomCard(const RoomCard& original);
    ~RoomCard() = default;
    
    
    void render(SDLContext& context);
    
    Card* clone() const;
    
private:
    void startDrag();
    void stopDrag();
    
    bool handleMouseMove(const SDL_MouseMotionEvent motion);
    
    avl::Vec2<int> mousePos;
    
    LocationMap potentialLocations;
    bool previewLocationValid;
    LocationID previewLocation;
    
    std::unique_ptr<Room> room;
    Fort& fort;
};


#endif	/* CARD_HPP */

