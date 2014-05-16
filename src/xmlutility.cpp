#include"../include/xmlutility.hpp"
#include"../include/objectmanager.hpp"
#include"../include/room.hpp"
#include"../include/card.hpp"
#include<pugixml.hpp>
#include<string>

using namespace std;
using namespace pugi;
using namespace avl;

const string RoomTag = "Room";
const string IDTag = "ID";
const string TextureTag = "Texture";
const string ExitTag = "Exit";
const string CardTag = "Card";
const string CardTypeTag = "Type";


RoomLoader::RoomLoader(SDLTextureManager& textureManager)
: textureManager(textureManager)
{
}

ObjectManager<Room> RoomLoader::load(const string& file) {
    return loadRoomsFromFile(textureManager, file);
}


CardLoader::CardLoader(SDLTextureManager& textureManager, const ObjectManager<Room>& rooms)
    : textureManager(textureManager), rooms(rooms)
{
}

ObjectManager<Card> CardLoader::load(const string& file) {
    
}


ObjectManager<Room> loadRoomsFromFile(SDLTextureManager& textureManager, const string& file) {
    xml_document doc = loadXMLFile(file);
    
    ObjectManager<Room> rooms;
    
    xml_node roomNode = doc.child(RoomTag);
    while(roomNode == true) {
        Room* room = loadRoom(file, textureManager, roomNode);
        
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

xml_document loadXMLFile(const string& file) {
    xml_document doc;
    
    xml_parse_result result = doc.load_file(file);
    
    if(result == false) {
        throw XMLException(__FILE__, __LINE__, file, result.description());
    }
    
    return doc;
}

Room* loadRoom(const string& file, SDLTextureManager& textureManager, xml_node& roomNode) {
    ExitType exits = ET_None;
    RoomID id;
    SDL_Texture* texture = nullptr;
    
    xml_node idNode = roomNode.child(IDTag);
    if(idNode == false) {
        throw XMLException(__FILE__, __LINE__, file, "Room doesn't have a node element.");
    }
    
    id = static_cast<RoomID>(idNode.text().as_uint());
    
    xml_node textureNode = roomNode.child(TextureTag);
    if(textureNode == false) {
        throw XMLException(__FILE__, __LINE__, file, "Room doesn't have a texture element.");
    }
    
    const string textureFile = textureNode.value();
    
    const TextureID textureID = textureManager.load(textureFile);
    texture = textureManager.getByID(textureID);
    
    xml_node exitNode = roomNode.child(ExitTag);
    while(exitNode == true) {
        exits |= toExitType(exitNode.value());
    }
    
    const Room* room = new Room(exits, id, texture);
    if(room == nullptr) {
        throw OutOfMemoryException(__FILE__, __LINE__);
    }
    return room;
}

ObjectManager<Card> loadCardsFromFile(SDLTextureManager& textureManager, const ObjectManager<Room>& rooms, const string& file) {
    xml_document doc = loadXMLFile(file);
    
    ObjectManager<Card> cards;
    
    xml_node cardNode = doc.child(CardTag);
    while(cardNode == true) {
        Card* card = loadCard(file, textureManager, rooms, cardNode);
        
        try {
            cards.add(card->getID(), card);
        } catch(...) {
            delete card;
            throw;
        }
        
        cardNode = cardNode.next_sibling(CardTag);
    }
    
    return cards;
}


Card* loadCard(const string& file, SDLTextureManager& textureManager, const ObjectManager<Room>& rooms, xml_node& cardNode) {
    CardID id = 0;
    SDL_Texture* texture;
    CardType type;
    
    
}