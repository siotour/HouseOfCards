#include"../include/room.hpp"
#include"../include/base.hpp"
#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<algorithm>

using namespace avl;


ExitType toExitType(std::string exit) {
    std::transform(exit.begin(), exit.end(), exit.begin(), ::tolower);
    
    ExitType result = ET_None;
    
    if(exit == "up") {
        result = ET_Up;
    } else if(exit == "down") {
        result = ET_Down;
    } else if(exit == "left") {
        result = ET_Left;
    } else if(exit == "right") {
        result = ET_Right;
    }
    
    return result;
}


Room::Room(const ExitType exits, RoomID id, SDL_Texture* const texture)
: dead(false), location({0, 0, 0, 0}), exits(exits), id(id), texture(texture)
{
}

Room::Room(const Room& original)
: dead(original.dead), location(original.location), exits(original.exits), id(original.id), texture(original.texture)
{
}

Room::~Room() {
}

bool Room::isDead() const {
    return dead;
}

void Room::update(const double deltaTime) {
    
}

void Room::render(SDLContext& context) {
    RenderCopy(context, texture, NULL, &location);
}

bool Room::handleEvent(const Event& event) {
    bool eventHandled = false;
    
    if(event.type == ET_MouseClick) {
        eventHandled = handleMouseButton(event.mouseClick);
    }
    
    return eventHandled;
}

ExitType Room::getExits() const {
    return exits;
}

RoomID Room::getID() const {
    return id;
}

SDL_Texture* Room::getTexture() const {
    return texture;
}

const Location& Room::getLocation() const {
    return location;
}

void Room::setLocation(const Location& newLocation) {
    location = newLocation;
}

bool Room::handleMouseButton(const MouseClickEvent button) {
    bool eventHandled = false;
    
    if(location.contains(button.relPos) == true) {
        dead = true;
        eventHandled = true;
    }
    
    return eventHandled;
}