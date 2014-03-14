#ifndef OBJECT_HPP
#define	OBJECT_HPP

#include"sdlutility.hpp"
//#include"events.hpp"
#include<SDL2/SDL.h>

class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    Object& operator=(const Object& rhs) = default;
    
    virtual void update(const double deltaTime);
    
    virtual void render(SDLContext& context);
    virtual bool handleEvent(const SDL_Event& event);
};


#endif	/* OBJECT_HPP */

