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
#include"fort.hpp"
#include<avl/include/exceptions.hpp>
#include<pugixml.hpp>
#include<string>
#include<memory>
#include<vector>


SDLContext* loadSettings(const std::string& file);

class RoomLoader {
public:
    RoomLoader(SDLTextureManager& textureManager);
    ~RoomLoader() = default;
    
    ObjectManager<Room> load(const std::string& fileName);
    
private:
    ObjectManager<Room> loadRoomsFromFile();
    Room* loadRoom(pugi::xml_node& roomNode);
    
    std::string file;
    SDLTextureManager& textureManager;
};


class CardLoader {
public:
    CardLoader(SDLTextureManager& textureManager, const ObjectManager<Room>& rooms, Fort& fort);
    ~CardLoader() = default;
    
    std::vector<std::shared_ptr<Card>> load(const std::string& fileName);
    
private:
    Card* loadCard(pugi::xml_node& cardNode);
    Card* loadRoomCard(const CardID id, SDL_Texture* texture, pugi::xml_node& cardNode);
    
    std::string file;
    SDLTextureManager& textureManager;
    const ObjectManager<Room>& rooms;
    Fort& fort;
};

class XMLException: public avl::Exception {
public:
    XMLException(const std::string& file, const unsigned int line, const std::string& xmlFile, const std::string& errorMessage);
    virtual ~XMLException() = default;
    
    const std::string& getXMLFile() const;
    const std::string& getError() const;
private:
    const std::string& xmlFile;
    const std::string& errorMessage;
};


#endif	/* XMLUTILITY_HPP */

