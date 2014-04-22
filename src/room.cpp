#include"../include/room.hpp"
#include"../include/base.hpp"
#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>


Room::Room(const Location& location, const ExitType exits, SDL_Texture* const texture)
: location(location), exits(exits), texture(texture)
{
}

Room::~Room() {
}

void Room::update(const double deltaTime) {
    
}

void Room::render(SDLContext& context) {
    SDL_RenderCopy(context.getRenderer(), texture, NULL, &location);
}

bool Room::handleEvent(const SDL_Event& event) {
    return false;
}

ExitType Room::getExits() const {
    return exits;
}

const Location& Room::getLocation() const {
    return location;
}

void Room::setLocation(const Location& newLocation) {
    location = newLocation;
}