#include"../include/xmlutility.hpp"
#include"../include/objectmanager.hpp"
#include"../include/room.hpp"
#include"../include/card.hpp"
#include<pugixml.hpp>
#include<string>
// For testing
#include<iostream>
#include<vector>

using namespace std;
using namespace pugi;
using namespace avl;

const char IndexTag[] = "Index";
const char RoomTag[] = "Room";
const char IDTag[] = "ID";
const char RoomIDTag[] = "RoomID";
const char TextureTag[] = "Texture";
const char ExitTag[] = "Exit";
const char CardTag[] = "Card";
const char CardTypeTag[] = "Type";


void loadXMLFile(xml_document& doc, const string& file) {
    xml_parse_result result = doc.load_file(file.c_str());
    
    if(result == false) {
        throw XMLException(__FILE__, __LINE__, file, result.description());
    }
}


RoomLoader::RoomLoader(SDLTextureManager& textureManager)
: textureManager(textureManager)
{
}

ObjectManager<Room> RoomLoader::load(const string& file) {
    xml_document doc;
    loadXMLFile(doc, file);
    
    ObjectManager<Room> rooms;
    
    xml_node indexNode = doc.child(IndexTag);
    xml_node roomNode = indexNode.child(RoomTag);
    while(roomNode.empty() == false) {
        Room* room = loadRoom(roomNode);
        
        try {
            rooms.add(room->getID(), room);
        } catch(...) {
            delete room;
            throw;
        }
        
        roomNode = roomNode.next_sibling(RoomTag);
    }
    
    return rooms;
}



Room* RoomLoader::loadRoom(xml_node& roomNode) {
    ExitType exits = ET_None;
    RoomID id;
    SDL_Texture* texture = nullptr;
    
    xml_node idNode = roomNode.child(IDTag);
    if(idNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Room doesn't have a node element.");
    }
    
    id = static_cast<RoomID>(idNode.text().as_uint());
    
    xml_node textureNode = roomNode.child(TextureTag);
    if(textureNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Room doesn't have a texture element.");
    }
    
    const string textureFile(textureNode.child_value());
    const TextureID textureID = textureManager.load(textureFile);
    texture = textureManager.getByID(textureID);
    
    xml_node exitNode = roomNode.child(ExitTag);
    while(exitNode.empty() == false) {
        exits = static_cast<ExitType>(exits | toExitType(exitNode.child_value()));
        exitNode = exitNode.next_sibling(ExitTag);
    }
    
    Room* const room = new Room(exits, id, texture);
    if(room == nullptr) {
        throw OutOfMemoryException(__FILE__, __LINE__);
    }
    return room;
}

CardLoader::CardLoader(SDLTextureManager& textureManager, const ObjectManager<Room>& rooms, Fort& fort)
    : textureManager(textureManager), rooms(rooms), fort(fort)
{
}

vector<shared_ptr<Card>> CardLoader::load(const std::string& fileName) {
    file = fileName;
    
    xml_document doc;
    loadXMLFile(doc, file);
    
    vector<shared_ptr<Card>> cards;
    
    xml_node indexNode = doc.child(IndexTag);
    xml_node cardNode = indexNode.child(CardTag);
    while(cardNode.empty() == false) {
        Card* card = loadCard(cardNode);
        
        try {
            cards.push_back(shared_ptr<Card>(card));
        } catch(...) {
            delete card;
            throw;
        }
        
        cardNode = cardNode.next_sibling(CardTag);
    }
    
    return cards;
}

Card* CardLoader::loadCard(xml_node& cardNode) {
    CardID id = 0;
    SDL_Texture* texture;
    CardType type;
    
    xml_node idNode = cardNode.child(IDTag);
    if(idNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "A card is missing an ID value.");
    }
    
    id = static_cast<CardID>(idNode.text().as_uint());
    
    xml_node textureNode = cardNode.child(TextureTag);
    if(textureNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "A card is missing a texture value.");
    }
    
    const string textureFile(textureNode.child_value());
    const TextureID textureID = textureManager.load(textureFile);
    texture = textureManager.getByID(textureID);
    
    xml_node typeNode = cardNode.child(CardTypeTag);
    if(typeNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "A card is missing a type value.");
    }
    
    type = toCardType(typeNode.child_value());
    
    Card* card = nullptr;
    
    switch(type) {
        case CT_Room:
            card = loadRoomCard(id, texture, cardNode);
            break;
        default:
            throw XMLException(__FILE__, __LINE__, file, "A card has an invalid type value.");
    }
    
    return card;
}

Card* CardLoader::loadRoomCard(const CardID id, SDL_Texture* texture, xml_node& cardNode) {
    RoomID roomID;
    
    xml_node roomIDNode = cardNode.child(RoomIDTag);
    if(roomIDNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "A room card doesn't have a room ID value.");
    }
    
    roomID = static_cast<RoomID>(roomIDNode.text().as_uint());
    
    std::shared_ptr<Room> room = rooms.getByID(roomID).lock();
    
    if(!room) {
        throw NullPointerException(__FILE__, __LINE__);
    }
    
    return new RoomCard(id, texture, *room, fort);
}



XMLException::XMLException(const std::string& file, const unsigned int line, const std::string& xmlFile, const std::string& errorMessage)
:   Exception(file, line),
    xmlFile(xmlFile),
    errorMessage(errorMessage)
{
    Exception::message = "An error occurred while loading the XML file " + xmlFile + ": " + errorMessage + ".";
}

const std::string& XMLException::getXMLFile() const {
    return xmlFile;
}

const std::string& XMLException::getError() const {
    return errorMessage;
}