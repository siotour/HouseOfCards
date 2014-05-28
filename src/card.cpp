#include"../include/card.hpp"
#include"../include/object.hpp"
#include"../include/fort.hpp"
#include"../include/events.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

using namespace avl;

const Vec2<double> ThumbnailSize = {0.1074218, 0.2005208};
const Vec2<double> PreviewSize = {0.2148, 0.40104};
const Vec2<double> PreviewOffset = {-0.0537, -0.42708};
const Vec2<double> RoomSize = {0.083, 0.110677};


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
    AABB2<double> thumbnailRect;
    thumbnailRect.left = thumbnailPosition.x;
    thumbnailRect.top = thumbnailPosition.y;
    thumbnailRect.right = thumbnailRect.left + ThumbnailSize.x;
    thumbnailRect.bottom = thumbnailRect.top + ThumbnailSize.y;
    
    RenderCopy(context, texture, NULL, &thumbnailRect);
    
    // Draw the preview if we're showing it
    if(previewOn == true) {        
        AABB2<double> previewRect;
        previewRect.left = thumbnailPosition.x + PreviewOffset.x;
        previewRect.top = thumbnailPosition.y + PreviewOffset.y;
        previewRect.right = previewRect.left + PreviewSize.x;
        previewRect.bottom = previewRect.top + PreviewSize.y;
        
        RenderCopy(context, texture, NULL, &previewRect);
    }
}

bool Card::handleEvent(const Event& event) {
    bool eventHandled = false;
    
    if(event.type == ET_MouseMove) {
        eventHandled = handleMouseMove(event.mouseMove);
    } else if(event.type == ET_MouseClick) {
        eventHandled = handleMouseButton(event.mouseClick);
    } else if(event.type == ET_KeyPress) {
        handleKey(event.keyPress);
    }
    
    return eventHandled;
}

void Card::setPosition(avl::Vec2<double> newPosition) {
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

bool Card::handleMouseMove(const MouseMoveEvent motion) {
    if(motion.relPos.x >= thumbnailPosition.x &&
       motion.relPos.x <= thumbnailPosition.x + ThumbnailSize.x &&
       motion.relPos.y >= thumbnailPosition.y &&
       motion.relPos.y <= thumbnailPosition.y + ThumbnailSize.y) {
        showPreview();
    } else {
        hidePreview();
    }
    
    // Always pass a mouse movement event on to other objects
    return false;
}


bool Card::handleMouseButton(const MouseClickEvent button) {
    bool eventHandled = false;
    
    if(button.button == MB_Left) {
        if(button.pressed == false) {
            if(isBeingDragged == true) {
                stopDrag();
                eventHandled = true;
            }
        } else if (button.relPos.x >= thumbnailPosition.x &&
                   button.relPos.x <= thumbnailPosition.x + ThumbnailSize.x &&
                   button.relPos.y >= thumbnailPosition.y &&
                   button.relPos.y <= thumbnailPosition.y + ThumbnailSize.y) {
            startDrag();
            eventHandled = true;
        }
    }
    
    return eventHandled;
}

bool Card::handleKey(const KeyPressEvent key) {
    bool eventHandled = false;
    
    if(isBeingDragged == true &&
       key.key.sym == SDLK_ESCAPE &&
       key.pressed == true) {
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
        AABB2<double> rect;
        rect.left = mousePos.x - RoomSize.x / 2;
        rect.top = mousePos.y - RoomSize.y / 2;
        rect.right = rect.left + RoomSize.x;
        rect.bottom = rect.top + RoomSize.y;
        RenderCopy(context, room->getTexture(), nullptr, &rect);
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

bool RoomCard::handleMouseMove(const MouseMoveEvent motion) {
    mousePos = motion.relPos;
    
    bool eventHandled = false;
    
    if(isBeingDragged == true) {
        eventHandled = true;
        const Vec2<double> pos = motion.relPos;

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