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
const string BackgroundImage = "assets/textures/battlebackground.png";
const string DeckButtonInactive = "assets/textures/deckbuttoninactive.png";
const string DeckButtonActive = "assets/textures/deckbuttonactive.png";
const string QuitButtonInactive = "assets/textures/quitbuttoninactive.png";
const string QuitButtonActive = "assets/textures/quitbuttonactive.png";
const string Card1Thumbnail = "assets/textures/card1thumb.png";
const string Card2Thumbnail = "assets/textures/card2thumb.png";
const string Card3Thumbnail = "assets/textures/card3thumb.png";
const string RoomHighlight = "assets/textures/roomHighlight.png";
const string Room1Image = "assets/textures/room1.png";
const string Room2Image = "assets/textures/room2.png";
const string Room3Image = "assets/textures/room3.png";

SDLTextureManager::ID BackgroundTexID;
SDLTextureManager::ID DeckInactiveTexID;
SDLTextureManager::ID DeckActiveTexID;
SDLTextureManager::ID QuitInactiveTexID;
SDLTextureManager::ID QuitActiveTexID;
SDLTextureManager::ID RoomHighlightTexID;

const AABB2<double> BackgroundPos = {0, 0, 1, 1};

const AABB2<double> DeckButtonPos = {0.78125, 0.74219, 0.8496118, 0.9817708};
const AABB2<double> QuitButtonPos = {0.85938, 0.88867, 0.9570293, 0.9505258};

const unsigned int MaxCards = 6;
// Hand of cards top/left corner
const Vec2<double> HandPosition = {0.009766, 0.78125};
const double CardSpacing = 0.15625;

} // Anonymous namespace



BattleScene::BattleScene(SDLContext& context)
: sceneDone(false), textureManager(move(unique_ptr<SDLTextureLoader>(new SDLTextureLoader(context))))
{
    loadTextures();
    
    background = textureManager.getByID(BackgroundTexID);
    
    SDL_Texture* deckInactive = textureManager.getByID(DeckInactiveTexID);
    SDL_Texture* deckActive = textureManager.getByID(DeckActiveTexID);
    deckButton.reset(new Button(DeckButtonPos, deckInactive, deckActive));
    
    SDL_Texture* quitInactive = textureManager.getByID(QuitInactiveTexID);
    SDL_Texture* quitActive = textureManager.getByID(QuitActiveTexID);
    quitButton.reset(new Button(QuitButtonPos, quitInactive, quitActive));
    
    SDL_Texture* roomHighlight = textureManager.getByID(RoomHighlightTexID);
    fort.reset(new Fort(roomHighlight));
    
    loadCards();
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
    
    fort->update(deltaTime);
}

void BattleScene::render(SDLContext& context) {
    RenderCopy(context, background, NULL, &BackgroundPos);
    deckButton->render(context);
    quitButton->render(context);
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

void BattleScene::loadTextures() {
    BackgroundTexID = textureManager.load(BackgroundImage);
    DeckInactiveTexID = textureManager.load(DeckButtonInactive);
    DeckActiveTexID = textureManager.load(DeckButtonActive);
    QuitInactiveTexID = textureManager.load(QuitButtonInactive);
    QuitActiveTexID = textureManager.load(QuitButtonActive);
    RoomHighlightTexID = textureManager.load(RoomHighlight);
}

void BattleScene::loadCards() {
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
