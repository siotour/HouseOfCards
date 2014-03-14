#include"../include/buttonobject.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include<iostream>


using namespace std;

ButtonObject::ButtonObject(const SDL_Rect& position, SDLContext& context, const string& inactiveImage, const string& activeImage)
: bounds(position)
{
    inactiveTexture = IMG_LoadTexture(context.getRenderer(), inactiveImage.c_str());
    if(inactiveTexture == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    activeTexture = IMG_LoadTexture(context.getRenderer(), activeImage.c_str());
    if(inactiveTexture == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
}

ButtonObject::~ButtonObject() {
    cleanup();
}

void ButtonObject::render(SDLContext& context) {
    if(isActive == true) {
        SDL_RenderCopy(context.getRenderer(), activeTexture, NULL, &bounds);
    } else {
        SDL_RenderCopy(context.getRenderer(), inactiveTexture, NULL, &bounds);
    }
}

bool ButtonObject::handleEvent(const SDL_Event& event) {
    bool isClicked = false;
    
    if(event.type == SDL_MOUSEMOTION) {
        handleMouseMove(event.motion.x, event.motion.y);
    } else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button == SDL_BUTTON_LEFT && isActive == true) {
            isClicked = true;
        }
    }
    
    return isClicked;
}

void ButtonObject::handleMouseMove(const short xPos, const short yPos) {
    if(xPos >= bounds.x && xPos <= bounds.x + bounds.w &&
       yPos >= bounds.y && yPos <= bounds.y + bounds.h) {
        isActive = true;
    } else {
        isActive = false;
    }
}

void ButtonObject::cleanup() {
    if(inactiveTexture != nullptr) {
        SDL_DestroyTexture(inactiveTexture);
        inactiveTexture = nullptr;
    }
    if(activeTexture != nullptr) {
        SDL_DestroyTexture(activeTexture);
        activeTexture = nullptr;
    }
}
