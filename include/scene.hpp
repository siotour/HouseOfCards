/* 
 * File:   scene.hpp
 * Author: sbachstein
 *
 * Created on March 12, 2014, 11:40 PM
 */

#ifndef SCENE_HPP
#define	SCENE_HPP

#include"object.hpp"
#include"SDL2/SDL.h"
#include<cstdint>

typedef uint8_t SceneType;

class Scene: public Object {
public:
    Scene();
    virtual ~Scene();
    Scene& operator=(const Scene& rhs);

    virtual void isDone() = 0;
    virtual SceneType getNextSceneType() = 0;
        
};



#endif	/* SCENE_HPP */

