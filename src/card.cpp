#include"../include/card.hpp"
#include"../include/object.hpp"
#include"../include/fort.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

using namespace avl;

Vec2<short> ThumbnailSize = {110, 154};
Vec2<short> PreviewSize = {220, 308};
Vec2<short> PreviewOffset = {-55, -328};


Card::Card(Vec2<short> position, SDL_Texture* const thumbnail, SDL_Texture* const preview)
: thumbnailPosition(position), isBeingDragged(false), previewOn(false), cardThumbnail(thumbnail), cardPreview(preview)
{
}

Card::~Card(){
}

void Card::update(const double deltaTime) {
    
}

void Card::render(SDLContext& context) {
    SDL_Rect thumbnailRect = {thumbnailPosition.x,
                              thumbnailPosition.y,
                              ThumbnailSize.x,
                              ThumbnailSize.y};
    
    SDL_RenderCopy(context.getRenderer(), cardThumbnail, NULL, &thumbnailRect);
    
    // Draw the preview if we're showing it
    if(previewOn == true) {        
        SDL_Rect previewRect = {thumbnailPosition.x + PreviewOffset.x,
                                thumbnailPosition.y + PreviewOffset.y,
                                PreviewSize.x,
                                PreviewSize.y};
        
        SDL_RenderCopy(context.getRenderer(), cardPreview, NULL, &previewRect);
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

void Card::cleanup() {
    
}



RoomCard::RoomCard(avl::Vec2<short> position, SDL_Texture* const thumbnail, SDL_Texture* const preview, Fort& fort)
    : Card(position, thumbnail, preview),
      fort(fort)
{
}

void RoomCard::startDrag() {
    Card::startDrag();
    potentialLocations = fort.showRoomLocations(id);
}

void RoomCard::stopDrag() {
    Card::stopDrag();
    potentialLocations.clear();
    fort.hideRoomLocations();
    fort.hideRoomPreview();
}

bool RoomCard::handleMouseMove(const SDL_MouseMotionEvent motion) {
    bool eventHandled = false;
    
    if(isBeingDragged == true) {
        eventHandled = true;
        const Vec2<int> pos = {motion.x, motion.y};

        for(auto currentLocation: potentialLocations) {
            if(currentLocation.second.contains(pos) == true) {
                fort.showRoomPreview(currentLocation.first, id);
                break;
            }
        }
    }
    
    return eventHandled;
}