#include"../include/minion.hpp"
#include"../include/object.hpp"
#include"../include/sprite.hpp"
#include"../include/sdlutility.hpp"
#include"../include/xmlutility.hpp"
#include<avl/include/utility.hpp>
#include<memory>
#include<string>
#include<cmath>
#include<algorithm>

using namespace std;
using namespace avl;


const AnimationID StandIdleAnimation = 0;
const AnimationID WalkIdleAnimation = 1;
const AnimationID JumpAnimation = 3;
const AnimationID JumpIdleAnimation = 4;

const string MinionSpriteFile = "assets/data/ninjaSprite.xml";

const Vec2<double> Speed = {0.08, 0.15};
const Vec2<double> Size = {0.02, 0.052};
const Vec2<double> HotSpot = {0.5, 0.897727};


Minion::Minion(SDLTextureManager& textureManager) {
    SpriteLoader spriteLoader(textureManager);
    
    sprite.reset(new Sprite(spriteLoader.load(MinionSpriteFile)));
    sprite->setDepth(0.5);
    
    sprite->setPosition({0.5, 0.5, 0.6, 0.53});
}

Minion::Minion(const Minion& original)
: isJumping(original.isJumping),
        isWalking(original.isWalking),
        position(original.position),
        targetPosition(original.targetPosition),
        sprite(new Sprite(*original.sprite))
{
}

void Minion::update(const double deltaTime) {
    // How far do we need to move?
    Vec2<double> targetOffset = targetPosition - position;
    
    // How far can we move in this timestep?
    Vec2<double> movement = {Speed.x * deltaTime, Speed.y * deltaTime};
    
    // Don't move past our target
    movement.x = min(movement.x, abs(targetOffset.x));
    movement.y = min(movement.y, abs(targetOffset.y));
    
    // Are we moving forward or backward in this direction?
    if(targetOffset.x < 0) {
        movement.x *= -1;
    }
    if(targetOffset.y < 0) {
        movement.y *= -1;
    }
    
    position += movement;
    AABB2<double> spritePos;
    spritePos.left = position.x - HotSpot.x * Size.x;
    spritePos.top = position.y - HotSpot.y * Size.y;
    spritePos.right = spritePos.left + Size.x;
    spritePos.bottom = spritePos.top + Size.y;
    
    sprite->setPosition(spritePos);
    
    if(movement.y != 0) {
        // Jump animation
        if(isJumping == false) {
            isJumping = true;
            isWalking = false;
            sprite->setAnimation(JumpAnimation);
            sprite->setIdleAnimation(JumpIdleAnimation);
        }
    } else if(movement.x != 0) {
        // Walk animation
        if(isWalking == false) {
            isWalking = true;
            isJumping = false;
            sprite->setAnimation(WalkIdleAnimation);
            sprite->setIdleAnimation(WalkIdleAnimation);
        }
    } else {
        // Stand animation
        isJumping = false;
        isWalking = false;
        sprite->setAnimation(StandIdleAnimation);
        sprite->setIdleAnimation(StandIdleAnimation);
    }
    
    sprite->update(deltaTime);
}

void Minion::render(SDLContext& context) {
    sprite->render(context);
}

bool Minion::handleEvent(const Event& event) {
    return false;
}

const Vec2<double>& Minion::getPosition() const {
    return position;
}

void Minion::setPosition(const avl::Vec2<double>& newPosition) {
    position = newPosition;
}

void Minion::moveTo(const Vec2<double>& newPosition) {
    targetPosition = newPosition;
}