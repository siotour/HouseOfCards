#include"../include/sprite.hpp"
#include"../include/object.hpp"
#include"../include/sdlutility.hpp"
#include"../include/events.hpp"
#include<avl/include/utility.hpp>
#include<map>

using namespace std;
using namespace avl;


FrameSet::FrameSet(const int numColumns, const int numRows)
: numColumns(numColumns), numRows(numRows)
{
    //avlAssert(numColumns >= 1);
    //avlAssert(numRows >= 1);
    frameWidth = 1.0 / static_cast<double>(numColumns);
    frameHeight = 1.0 / static_cast<double>(numRows);
}

const AABB2<double> FrameSet::getFrameBounds(const FrameID frame) const {
    // avlAssert(frame < numColumns * numRows);
    const int frameNum = static_cast<const int>(frame);
    
    AABB2<double> bounds;
    
    bounds.left = frameWidth * (frameNum % numColumns);
    bounds.right = bounds.left + frameWidth;
    bounds.top = frameHeight * (frameNum / numColumns);
    bounds.bottom = bounds.top + frameHeight;
    
    return bounds;
}



Sprite::Sprite(SDL_Texture* const texture, const AnimationMap& animations, const int numColumns, const int numRows)
: frameSet(numColumns, numRows), animations(animations), texture(texture)
{
    setIdleAnimation(0);
    setAnimation(0);
}

double Sprite::getDepth() const {
    return depth;
}

void Sprite::setDepth(const double newDepth) {
    depth = newDepth;
}

const AABB2<double>& Sprite::getPosition() const {
    return position;
}

void Sprite::setPosition(const avl::AABB2<double>& newPosition) {
    position = newPosition;
}

void Sprite::setIdleAnimation(const AnimationID id) {
    idleAnimation = id;
}

void Sprite::setAnimation(const AnimationID id) {
    currentAnimation = id;
    currentFrame = 0;
}

void Sprite::update(const double deltaTime){
    accumulatedTime += deltaTime;
    
    Animation animation = animations[currentAnimation];
    
    if(accumulatedTime >= animation.frameDelay) {
        accumulatedTime -= animation.frameDelay;
        ++currentFrame;
        if(currentFrame >= animation.startFrame + animation.numFrames) {
            setAnimation(idleAnimation);
        }
    }
}

void Sprite::render(SDLContext& context) {
    Animation animation = animations[currentAnimation];
    AABB2<double> frameBounds = frameSet.getFrameBounds(animation.startFrame + currentFrame);
    
    context.renderTexture(texture, &frameBounds, &position, depth);
}