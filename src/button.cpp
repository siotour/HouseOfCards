#include"../include/button.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>


using namespace std;
using namespace avl;

Button::Button(const AABB2<double>& position, SDL_Texture* const inactiveTexture, SDL_Texture* const activeTexture)
: isActive(false), bounds(position), inactiveTexture(inactiveTexture), activeTexture(activeTexture)
{
}

void Button::render(SDLContext& context) {
    if(isActive == true) {
        RenderCopy(context, activeTexture, nullptr, &bounds);
    } else {
        RenderCopy(context, inactiveTexture, nullptr, &bounds);
    }
}

bool Button::handleEvent(const Event& event) {
    bool isClicked = false;
    
    if(event.type == ET_MouseMove) {
        handleMouseMove(event.mouseMove.relPos);
    } else if(event.type == ET_MouseClick) {
        if(event.mouseClick.button == MB_Left && event.mouseClick.pressed == true && isActive == true) {
            isClicked = true;
        }
    }
    
    return isClicked;
}

void Button::handleMouseMove(const Vec2<double>& pos) {
    if(bounds.contains(pos) == true) {
        isActive = true;
    } else {
        isActive = false;
    }
}
