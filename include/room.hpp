/* 
 * File:   room.hpp
 * Author: sbachstein
 *
 * Created on April 17, 2014, 4:52 PM
 */

#ifndef ROOM_HPP
#define	ROOM_HPP

#include"object.hpp"
#include"base.hpp"
#include<string>
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>

typedef unsigned short RoomID;

enum ExitType {
    ET_None     = 0x0,
    ET_Left     = 0x1,
    ET_Right    = 0x2,
    ET_Up       = 0x4,
    ET_Down     = 0x8
};

ExitType toExitType(std::string exit);


class Room: public Object {
public:
    Room(const ExitType exits, RoomID id, SDL_Texture* const texture);
    Room(const Room& original);
    ~Room();
    
    bool isDead() const;
    
    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const Event& event);
    
    ExitType getExits() const;
    RoomID getID() const;
    SDL_Texture* getTexture() const;
    const avl::Vec2<double> getWaypoint() const;
    const avl::AABB2<double>& getLocation() const;
    void setLocation(const avl::AABB2<double>& newLocation);
    
    
private:
    bool handleMouseButton(const MouseClickEvent button);
    
    
    bool dead;
    avl::AABB2<double> location;
    ExitType exits;
    RoomID id;
    
    
    SDL_Texture* texture;
    
};


#endif	/* ROOM_HPP */

