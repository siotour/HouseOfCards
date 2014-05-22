#include"../include/events.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>

bool makeEvent(const SDLContext& context, const SDL_Event sdlEvent, Event& event) {
    bool eventModified = true;
    
    switch(sdlEvent.type) {
        case SDL_MOUSEMOTION:
            event.type = ET_MouseMove;
            break;
        case SDL_MOUSEBUTTONDOWN:
            // Falls through
        case SDL_MOUSEBUTTONUP:
            event.type = ET_MouseClick;
            break;
        case SDL_MOUSEWHEEL:
            event.type = ET_MouseScroll;
            break;
        case SDL_WINDOWEVENT:
            if(sdlEvent.window.type == )
            event.type = ET_Application;
            event.application.active = sdlEvent.active.gain;
            break;
        case SDL_KEYDOWN:
            // Falls through
        case SDL_KEYUP:
            event.type = ET_KeyPress;
            event.keyPress.key = sdlEvent.key.keysym;
            event.keyPress.pressed = (sdlEvent.key.type == SDL_KEYDOWN) ? true : false;
            break;
        case SDL_QUIT:
            event.type = ET_Quit;
            break;
        default:
            // No event update
            eventModified = false;
    }
    
    return eventModified;
}
