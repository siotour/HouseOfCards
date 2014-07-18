#include"../include/battlescene.hpp"
#include"../include/scene.hpp"
#include"../include/card.hpp"
#include"../include/button.hpp"
// For SceneType definitions...
#include"../include/mainmenu.hpp"
#include"../include/xmlutility.hpp"
#include<memory>
#include<vector>
#include<string>
#include<iterator>
#include<random>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>


using namespace std;
using namespace avl;

namespace {
default_random_engine rng;
uniform_int_distribution<int> dist(1, 3);

const string RoomIndex = "assets/data/roomIndex.xml";
const string CardIndex = "assets/data/cardIndex.xml";
const string BackgroundImage = "assets/textures/battleBackground.png";
const string Card1Thumbnail = "assets/textures/card1thumb.png";
const string Card2Thumbnail = "assets/textures/card2thumb.png";
const string Card3Thumbnail = "assets/textures/card3thumb.png";
const string RoomHighlight = "assets/textures/roomHighlight.png";
const string Room1Image = "assets/textures/room1.png";
const string Room2Image = "assets/textures/room2.png";
const string Room3Image = "assets/textures/room3.png";

const string DeckButtonPath = "assets/data/deckButton.xml";
const string QuitButtonPath = "assets/data/battleQuitButton.xml";
const string MinionButtonPath = "assets/data/minionButton.xml";
const string MusicPath = "assets/audio/Video Game - Trieste_0.ogg";

const string RoomBuildPath = "assets/audio/build.wav";
const string RoomDestroyPath = "assets/audio/destroy.wav";

SDLTextureManager::ID BackgroundTexID;
SDLTextureManager::ID RoomHighlightTexID;

const AABB2<double> BackgroundPos = {0, 0, 1, 1};

const AABB2<double> DeckButtonPos = {0.742188, 0.78125, 0.8496093, 0.9817708};
const AABB2<double> QuitButtonPos = {0.8886718, 0.85938, 0.9570312, 0.950521};
const AABB2<double> MinionButtonPos{0.8886718, 0.74, 0.9570312, 0.84};

const unsigned int MaxCards = 6;
// Hand of cards top/left corner
const Vec2<double> HandPosition = {0.009766, 0.78125};
const double CardSpacing = 0.117;

} // Anonymous namespace



BattleScene::BattleScene(SDLContext& context)
: sceneDone(false)
{
    SDLTextureManager& textureManager = context.getTextureManager();
    
    loadTextures(textureManager);
    
    background = textureManager.getByID(BackgroundTexID);
    
    deckButton.reset(fromXML(context, DeckButtonPath));
    deckButton->setPosition(DeckButtonPos);
    quitButton.reset(fromXML(context, QuitButtonPath));
    quitButton->setPosition(QuitButtonPos);
    minionButton.reset(fromXML(context, MinionButtonPath));
    minionButton->setPosition(MinionButtonPos);
    
    SDL_Texture* roomHighlight = textureManager.getByID(RoomHighlightTexID);
    
    auto soundID = context.getSoundManager().load(RoomBuildPath);
    auto buildSound = context.getSoundManager().getByID(soundID);
    soundID = context.getSoundManager().load(RoomDestroyPath);
    auto destroySound = context.getSoundManager().getByID(soundID);
    
    auto musicID = context.getMusicManager().load(MusicPath);
    music = context.getMusicManager().getByID(musicID);
    
    playMusic(music);
    
    Minion* minion = new Minion(textureManager);
    fort.reset(new Fort(roomHighlight, buildSound, destroySound, minion));
    
    loadCards(textureManager);
}

BattleScene::~BattleScene() {
    cleanup();
}

void BattleScene::update(const double deltaTime) {
    for(size_t i = 0; i < cards.size(); ++i) {
        cards[i]->update(deltaTime);
        if(cards[i]->isDead() == true) {
            deleteCard(i);
        }
    }
    
    deckButton->update(deltaTime);
    quitButton->update(deltaTime);
    minionButton->update(deltaTime);
    
    fort->update(deltaTime);
}

void BattleScene::render(SDLContext& context) {
    RenderCopy(context, background, NULL, &BackgroundPos);
    deckButton->render(context);
    quitButton->render(context);
    minionButton->render(context);
    fort->render(context);
    for(unique_ptr<Card>& card : cards) {
        card->render(context);
    }
}

bool BattleScene::handleEvent(const Event& event) {
    bool eventHandled = false;
    
    if(event.type == ET_Quit) {
        eventHandled = true;
        nextScene = ST_Quit;
        quit();
    } else if(deckButton->handleEvent(event) == true) {
        eventHandled = true;
        drawCard();
    } else if(quitButton->handleEvent(event) == true) {
        eventHandled = true;
        nextScene = ST_MainMenu;
        quit();
    } else if(minionButton->handleEvent(event) == true) {
        eventHandled = true;
        fort->spawnMinion();
    } else {
        for(size_t i = 0; i < cards.size(); ++i) {
            if(cards[i]->handleEvent(event) == true) {
                eventHandled = true;
                break;
            }
        }
        
        if(eventHandled == false) {
            eventHandled = fort->handleEvent(event);
        }
    }
    
    return eventHandled;
}

bool BattleScene::isDone() {
    return sceneDone;
}

SceneType BattleScene::getNextSceneType() {
    return nextScene;
}

void BattleScene::drawCard() {
    if(cards.size() < MaxCards) {
        Vec2<double> cardPos;
        cardPos.x = HandPosition.x + cards.size() * CardSpacing;
        cardPos.y = HandPosition.y;
        int cardID = dist(rng);
        Card* newCard = cardFactory.makeCard(cardID);
        newCard->setPosition(cardPos);
        cards.emplace_back(newCard);
    }
}

void BattleScene::deleteCard(const unsigned int index) {
    if(index < cards.size()) {
        auto iter = cards.begin();
        advance(iter, index);
        cards.erase(iter);
    }
    
    // Reposition cards to fill in the gap created
    Vec2<double> cardPos;
    cardPos.y = HandPosition.y;
    for(size_t i = index; i < cards.size(); ++i) {
        cardPos.x = HandPosition.x + i * CardSpacing;
        cards[i]->setPosition(cardPos);
    }
}

void BattleScene::quit() {
    sceneDone = true;
}

void BattleScene::loadTextures(SDLTextureManager& textureManager) {
    BackgroundTexID = textureManager.load(BackgroundImage);
    RoomHighlightTexID = textureManager.load(RoomHighlight);
}

void BattleScene::loadCards(SDLTextureManager& textureManager) {
    RoomLoader roomLoader(textureManager);
    
    ObjectManager<Room> rooms = roomLoader.load(RoomIndex);
    
    CardLoader cardLoader(textureManager, rooms, *fort);
    
    vector<shared_ptr<Card>> cards = cardLoader.load(CardIndex);
    
    auto i = cards.begin();
    while(i != cards.end()) {
        cardFactory.addCard((*i)->clone());
        cardIDs.push_back((*i)->getID());
        i = cards.erase(i);
    }
}

void BattleScene::cleanup() {
    cards.clear();
}
