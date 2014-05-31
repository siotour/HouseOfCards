/* 
 * File:   minion.hpp
 * Author: sbachstein
 *
 * Created on May 29, 2014, 11:33 AM
 */

#ifndef MINION_HPP
#define	MINION_HPP

#include"object.hpp"
#include"sprite.hpp"
#include"sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<memory>



class Minion : public Object {
public:
    Minion(SDLTextureManager& textureManager);
    virtual ~Minion() = default;
    
    virtual void update(const double deltaTime);
    
    virtual void render(SDLContext& context);
    virtual bool handleEvent(const Event& event);
    
    void moveTo(const avl::Vec2<double>& newPosition);
    
private:
    bool isJumping;
    bool isWalking;
    
    avl::Vec2<double> position;
    avl::Vec2<double> targetPosition;
    
    std::unique_ptr<Sprite> sprite;
    
};




#endif	/* MINION_HPP */

