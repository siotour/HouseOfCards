/* 
 * File:   sprite.hpp
 * Author: sbachstein
 *
 * Created on May 28, 2014, 12:52 PM
 */

#ifndef SPRITE_HPP
#define	SPRITE_HPP

#include"object.hpp"
#include"sdlutility.hpp"
#include"events.hpp"
#include<avl/include/utility.hpp>
#include<map>

typedef unsigned short AnimationID;
typedef unsigned short FrameID;

struct Animation {
    AnimationID id;
    FrameID startFrame;
    unsigned short numFrames;
    double frameDelay;
};


class FrameSet {
public:
    FrameSet(const int numColumns, const int numRows);
    ~FrameSet() = default;
    
    void setFrame(const FrameID frame);
    const avl::AABB2<double> getFrameBounds() const;
    
private:
    avl::AABB2<double> bounds;
    
    int numColumns;
    int numRows;
    double frameWidth;
    double frameHeight;
};


class Sprite : public Object {
public:
    Sprite(SDL_Texture* const texture, const AnimationMap& animations, const int numColumns, const int numRows);
    virtual ~Sprite() = default;
    
    double getDepth() const;
    void setDepth(const double newDepth);
    
    const avl::AABB2<double>& getPosition() const;
    void setPosition(const avl::AABB2<double>& newPosition);
    
    void setIdleAnimation(const AnimationID id);
    void setAnimation(const AnimationID id);
    
    void update(const double deltaTime);
    void render(SDLContext& context);
    
private:
    
    avl::AABB2<double> position;
    double depth;
    
    double accumulatedTime;
    FrameID currentFrame;
    
    AnimationID idleAnimation;
    AnimationID currentAnimation;
    
    FrameSet frameSet;
    
    typedef std::map<AnimationID, Animation> AnimationMap;
    AnimationMap animations;
    
    SDL_Texture* texture;
};

#endif	/* SPRITE_HPP */

