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

const char SettingsTag[] = "Settings";
const char TitleTag[] = "WindowTitle";
const char WidthTag[] = "Width";
const char HeightTag[] = "Height";
const char FullscreenTag[] = "Fullscreen";

const char IndexTag[] = "Index";
const char RoomTag[] = "Room";
const char IDTag[] = "ID";
const char RoomIDTag[] = "RoomID";
const char TextureTag[] = "Texture";
const char ExitTag[] = "Exit";
const char CardTag[] = "Card";
const char CardTypeTag[] = "Type";

const char SpriteTag[] = "Sprite";
const char NumColumnsTag[] = "NumColumns";
const char NumRowsTag[] = "NumRows";
const char AnimationTag[] = "Animation";
const char StartFrameTag[] = "StartFrame";
const char NumFramesTag[] = "NumFrames";
const char FrameDelayTag[] = "FrameDelay";



void loadXMLFile(xml_document& doc, const string& file) {
    xml_parse_result result = doc.load_file(file.c_str());
    
    if(result == false) {
        throw XMLException(__FILE__, __LINE__, file, result.description());
    }
}



SDLContext* loadSettings(const string& file) {
    xml_document doc;
    loadXMLFile(doc, file);
    
    xml_node settingsNode = doc.child(SettingsTag);
    if(settingsNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Missing settings element.");
    }
    
    xml_node titleNode = settingsNode.child(TitleTag);
    if(titleNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Missing title element.");
    }
    const string windowTitle(titleNode.child_value());
    
    xml_node widthNode = settingsNode.child(WidthTag);
    if(widthNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Missing width element.");
    }
    const unsigned int width = widthNode.text().as_uint();
    
    xml_node heightNode = settingsNode.child(HeightTag);
    if(heightNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Missing height element.");
    }
    const unsigned int height = heightNode.text().as_uint();
    
    xml_node fullscreenNode = settingsNode.child(FullscreenTag);
    if(fullscreenNode.empty() == true) {
        throw XMLException(__FILE__, __LINE__, file, "Missing fullscreen element.");
    }
    const bool fullscreen = fullscreenNode.text().as_bool();
    
    return new SDLContext(windowTitle, width, height, fullscreen);
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



SpriteLoader::SpriteLoader(SDLTextureManager& textureManager)
: textureManager(textureManager)
{
}

Sprite SpriteLoader::load(const std::string& fileName) {
    string file = fileName;
    
    xml_document doc;
    loadXMLFile(doc, file);
    
    xml_node spriteNode = doc.child(SpriteTag);
    
    xml_node textureNode = spriteNode.child(TextureTag);
    string textureFile = textureNode.child_value();
    TextureID textureID = textureManager.load(textureFile);
    SDL_Texture* texture = textureManager.getByID(textureID);
    
    xml_node numColNode = spriteNode.child(NumColumnsTag);
    const unsigned int numColumns = numColNode.text().as_uint();
    
    xml_node numRowNode = spriteNode.child(NumRowsTag);
    const unsigned int numRows = numRowNode.text().as_uint();
    
    AnimationMap animations;
    
    xml_node animationNode = spriteNode.child(AnimationTag);
    while(animationNode.empty() == false) {
        Animation animation = loadAnimation(animationNode);
        animations.insert(make_pair(animation.id, animation));
        animationNode = animationNode.next_sibling(AnimationTag);
    }
    
    return Sprite(texture, animations, numColumns, numRows);
}

Animation SpriteLoader::loadAnimation(xml_node& animationNode) {
    const AnimationID animationID = animationNode.child(IDTag).text().as_uint();
    const FrameID startFrame = animationNode.child(StartFrameTag).text().as_uint();
    const unsigned short numFrames = animationNode.child(NumFramesTag).text().as_uint();
    const double frameDelay = animationNode.child(FrameDelayTag).text().as_double();
    
    Animation animation = {animationID, startFrame, numFrames, frameDelay};
    return animation;
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