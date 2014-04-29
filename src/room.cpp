#include"../include/room.hpp"
#include"../include/base.hpp"
#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>

using namespace avl;


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
    SDL_RenderCopy(context.getRenderer(), texture, NULL, &location);
}

bool Room::handleEvent(const SDL_Event& event) {
    bool eventHandled = false;
    
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        eventHandled = handleMouseButton(event.button);
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

bool Room::handleMouseButton(const SDL_MouseButtonEvent button) {
    bool eventHandled = false;
    
    Vec2<int> clickPos = {button.x, button.y};
    
    if(location.contains(clickPos) == true) {
        dead = true;
        eventHandled = true;
    }
    
    return eventHandled;
}