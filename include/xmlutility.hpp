/* 
 * File:   xmlutility.hpp
 * Author: sbachstein
 *
 * Created on May 15, 2014, 3:06 PM
 */

#ifndef XMLUTILITY_HPP
#define	XMLUTILITY_HPP


#include"objectmanager.hpp"
#include"room.hpp"
#include"card.hpp"
#include"sdlutility.hpp"
#include<avl/include/exceptions.hpp>
#include<string>


class RoomLoader {
public:
    RoomLoader(SDLTextureManager& textureManager);
    ~RoomLoader() = default;
    
    ObjectManager<Room> load(const std::string& file);
    
private:
    SDLTextureManager& textureManager;
};


class CardLoader {
public:
    CardLoader(SDLTextureManager& textureManager, const ObjectManager<Room>& rooms);
    ~CardLoader() = default;
    
    ObjectManager<Card> load(const std::string& file);
    
private:
    SDLTextureManager& textureManager;
    const ObjectManager<Room>& rooms;
};

class XMLException: public avl::Exception {
public:
    
};


#endif	/* XMLUTILITY_HPP */

