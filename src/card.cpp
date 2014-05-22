#include"../include/card.hpp"
#include"../include/object.hpp"
#include"../include/fort.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

using namespace avl;

Vec2<short> ThumbnailSize = {110, 154};
Vec2<short> PreviewSize = {220, 308};
Vec2<short> PreviewOffset = {-55, -328};


CardType toCardType(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    
    CardType type = CT_Unknown;
    
    if(text == "room") {
        type = CT_Room;
    }
    
    return type;
}

Card::Card(const CardID id, SDL_Texture* const texture)
:   id(id),
    thumbnailPosition({0, 0}),
    dead(false),
    isBeingDragged(false), 
    previewOn(false), texture(texture)
{
}

Card::Card(const Card& original)
:   type(original.type),
    id(original.id), 
    thumbnailPosition(original.thumbnailPosition),
    dead(original.dead),
    isBeingDragged(original.isBeingDragged),
    previewOn(original.previewOn),
    texture(original.texture)
{
}

CardType Card::getType() const {
    return type;
}

CardID Card::getID() const {
    return id;
}

bool Card::isDead() const {
    return dead;
}

void Card::update(const double deltaTime) {
    
}

void Card::render(SDLContext& context) {
    SDL_Rect thumbnailRect = {thumbnailPosition.x,
                              thumbnailPosition.y,
                              ThumbnailSize.x,
                              ThumbnailSize.y};
    
    SDL_RenderCopy(context.getRenderer(), texture, NULL, &thumbnailRect);
    
    // Draw the preview if we're showing it
    if(previewOn == true) {        
        SDL_Rect previewRect = {thumbnailPosition.x + PreviewOffset.x,
                                thumbnailPosition.y + PreviewOffset.y,
                                PreviewSize.x,
                                PreviewSize.y};
        
        SDL_RenderCopy(context.getRenderer(), texture, NULL, &previewRect);
    }
}

bool Card::handleEvent(const SDL_Event& event) {
    bool eventHandled = false;
    
    if(event.type == SDL_MOUSEMOTION) {
        eventHandled = handleMouseMove(event.motion);
    } else if(event.type == SDL_MOUSEBUTTONDOWN) {
        eventHandled = handleMouseButton(event.button);
    } else if(event.type == SDL_MOUSEBUTTONUP) {
        eventHandled = handleMouseButton(event.button);
    } else if(event.type == SDL_KEYDOWN) {
        handleKey(event.key);
    }
    
    return eventHandled;
}

void Card::setPosition(avl::Vec2<short> newPosition) {
    thumbnailPosition = newPosition;
}

void Card::startDrag() {
    isBeingDragged = true;
}

void Card::stopDrag() {
    isBeingDragged = false;
}

void Card::showPreview() {
    previewOn = true;
}

void Card::hidePreview() {
    previewOn = false;
}

bool Card::handleMouseMove(const SDL_MouseMotionEvent motion) {
    if(motion.x >= thumbnailPosition.x &&
       motion.x <= thumbnailPosition.x + ThumbnailSize.x &&
       motion.y >= thumbnailPosition.y &&
       motion.y <= thumbnailPosition.y + ThumbnailSize.y) {
        showPreview();
    } else {
        hidePreview();
    }
    
    // Always pass a mouse movement event on to other objects
    return false;
}


bool Card::handleMouseButton(const SDL_MouseButtonEvent button) {
    bool eventHandled = false;
    
    if(button.state == SDL_RELEASED) {
        if(isBeingDragged == true) {
            stopDrag();
            eventHandled = true;
        }
    } else if (button.x >= thumbnailPosition.x &&
               button.x <= thumbnailPosition.x + ThumbnailSize.x &&
               button.y >= thumbnailPosition.y &&
               button.y <= thumbnailPosition.y + ThumbnailSize.y) {
        startDrag();
        eventHandled = true;
    }
    
    return eventHandled;
}

bool Card::handleKey(const SDL_KeyboardEvent key) {
    bool eventHandled = false;
    
    if(isBeingDragged == true &&
       key.keysym.sym == SDLK_ESCAPE &&
       key.state == SDL_PRESSED) {
        stopDrag();
        eventHandled = true;
    }
    
    return eventHandled;
}



RoomCard::RoomCard(const CardID id, SDL_Texture* const texture, const Room& room, Fort& fort)
    : Card(id, texture),
      previewLocationValid(false),
      room(new Room(room)),
      fort(fort)
{
}

RoomCard::RoomCard(const RoomCard& original)
:   Card(original),
    potentialLocations(original.potentialLocations),
    previewLocationValid(original.previewLocationValid),
    room(new Room(*original.room)),
    fort(original.fort)
{
}

void RoomCard::render(SDLContext& context) {
    Card::render(context);
    if(isBeingDragged == true && previewLocationValid == false) {
        SDL_Rect rect = {mousePos.x - 85 / 2, mousePos.y - 85 / 2, 85, 85};
        SDL_RenderCopy(context.getRenderer(), room->getTexture(), nullptr, &rect);
    }
}

Card* RoomCard::clone() const {
    return new RoomCard(*this);
}

void RoomCard::startDrag() {
    // Hide card preview
    hidePreview();
    Card::startDrag();
    potentialLocations = fort.showRoomLocations(*room);
}

void RoomCard::stopDrag() {
    Card::stopDrag();
    potentialLocations.clear();
    fort.hideRoomLocations();
    fort.hideRoomPreview();
    if(previewLocationValid == true) {
        fort.buildRoom(*room, previewLocation);
        dead = true;
    }
}

bool RoomCard::handleMouseMove(const SDL_MouseMotionEvent motion) {
    mousePos.x = motion.x;
    mousePos.y = motion.y;
    
    bool eventHandled = false;
    
    if(isBeingDragged == true) {
        eventHandled = true;
        const Vec2<int> pos = {motion.x, motion.y};

        bool displayPreview = false;
        for(auto currentLocation: potentialLocations) {
            if(currentLocation.second.contains(pos) == true) {
                fort.showRoomPreview(*room, currentLocation.first);
                previewLocation = currentLocation.first;
                previewLocationValid = true;
                displayPreview = true;
                break;
            }
        }
 
        if(displayPreview == false) {
            fort.hideRoomPreview();
            previewLocationValid = false;
        }
    } else {
        // Show card preview
        Card::handleMouseMove(motion);
    }
    
    return eventHandled;
}