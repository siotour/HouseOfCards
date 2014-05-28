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
#include"events.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<vector>
#include<memory>
#include<string>


typedef unsigned short CardID;

enum CardType {
    CT_Unknown,
    CT_Room
};

CardType toCardType(std::string text);


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
    virtual bool handleEvent(const Event& event);
    
    void setPosition(avl::Vec2<double> newPosition);
    
protected:
    Card(const CardID id, SDL_Texture* const texture);
    virtual void startDrag();
    virtual void stopDrag();
    void showPreview();
    void hidePreview();
    virtual bool handleMouseMove(const MouseMoveEvent motion);
    virtual bool handleMouseButton(const MouseClickEvent button);
    virtual bool handleKey(const KeyPressEvent key);
    
    CardType type;
    CardID id;
    
    avl::Vec2<double> thumbnailPosition;
    bool dead;
    bool isBeingDragged;
    bool previewOn;
    
    SDL_Texture* texture;
    
};


class RoomCard: public Card {
public:
    RoomCard(const CardID id, SDL_Texture* const texture, const Room& room, Fort& fort);
    RoomCard(const RoomCard& original);
    ~RoomCard() = default;
    
    
    void render(SDLContext& context);
    
    Card* clone() const;
    
private:
    void startDrag();
    void stopDrag();
    
    bool handleMouseMove(const MouseMoveEvent motion);
    
    avl::Vec2<double> mousePos;
    
    LocationMap potentialLocations;
    bool previewLocationValid;
    LocationID previewLocation;
    
    std::unique_ptr<Room> room;
    Fort& fort;
};


#endif	/* CARD_HPP */

