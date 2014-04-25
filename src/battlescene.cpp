#include"../include/battlescene.hpp"
#include"../include/scene.hpp"
#include"../include/card.hpp"
#include"../include/button.hpp"
// For SceneType definitions...
#include"../include/mainmenu.hpp"
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

const string BackgroundImage = "assets/textures/battlebackground.png";
const string DeckButtonInactive = "assets/textures/deckbuttoninactive.png";
const string DeckButtonActive = "assets/textures/deckbuttonactive.png";
const string QuitButtonInactive = "assets/textures/quitbuttoninactive.png";
const string QuitButtonActive = "assets/textures/quitbuttonactive.png";
const string Card1Thumbnail = "assets/textures/card1thumb.png";
const string Card1Preview = "assets/textures/card1preview.png";
const string Card2Thumbnail = "assets/textures/card2thumb.png";
const string Card2Preview = "assets/textures/card2preview.png";
const string Card3Thumbnail = "assets/textures/card3thumb.png";
const string Card3Preview = "assets/textures/card3preview.png";
const string RoomHighlight = "assets/textures/roomHighlight.png";
const string Room1Image = "assets/textures/room1.png";
const string Room2Image = "assets/textures/room2.png";
const string Room3Image = "assets/textures/room3.png";

SDLTextureManager::ID BackgroundTexID;
SDLTextureManager::ID DeckInactiveTexID;
SDLTextureManager::ID DeckActiveTexID;
SDLTextureManager::ID QuitInactiveTexID;
SDLTextureManager::ID QuitActiveTexID;
SDLTextureManager::ID Card1ThumbTexID;
SDLTextureManager::ID Card1PreviewTexID;
SDLTextureManager::ID Card2ThumbTexID;
SDLTextureManager::ID Card2PreviewTexID;
SDLTextureManager::ID Card3ThumbTexID;
SDLTextureManager::ID Card3PreviewTexID;
SDLTextureManager::ID RoomHighlightTexID;
SDLTextureManager::ID Room1TexID;
SDLTextureManager::ID Room2TexID;
SDLTextureManager::ID Room3TexID;

const SDL_Rect BackgroundPos = {0, 0, 1024, 768};

const SDL_Rect DeckButtonPos = {760, 600, 110, 154};
const SDL_Rect QuitButtonPos = {910, 660, 70, 70};

const unsigned int MaxCards = 6;
// Hand of cards top/left corner
const Vec2<short> HandPosition = {10, 600};
const short CardSpacing = 120;

} // Anonymous namespace



BattleScene::BattleScene(SDLContext& context)
: sceneDone(false), textureManager(new SDLTextureLoader(context))
{
    loadTextures();
    
    background = textureManager.getByID(BackgroundTexID);
    
    SDL_Texture* deckInactive = textureManager.getByID(DeckInactiveTexID);
    SDL_Texture* deckActive = textureManager.getByID(DeckActiveTexID);
    deckButton.reset(new Button(DeckButtonPos, deckInactive, deckActive));
    
    SDL_Texture* quitInactive = textureManager.getByID(QuitInactiveTexID);
    SDL_Texture* quitActive = textureManager.getByID(QuitActiveTexID);
    quitButton.reset(new Button(QuitButtonPos, quitInactive, quitActive));
    
    cardThumbnails[0] = textureManager.getByID(Card1ThumbTexID);
    cardThumbnails[1] = textureManager.getByID(Card2ThumbTexID);
    cardThumbnails[2] = textureManager.getByID(Card3ThumbTexID);
    
    cardPreviews[0] = textureManager.getByID(Card1PreviewTexID);
    cardPreviews[1] = textureManager.getByID(Card2PreviewTexID);
    cardPreviews[2] = textureManager.getByID(Card3PreviewTexID);
    
    SDL_Texture* roomHighlight = textureManager.getByID(RoomHighlightTexID);
    fort.reset(new Fort(roomHighlight));
    
    loadCards();
}

BattleScene::~BattleScene() {
    cleanup();
}

void BattleScene::update(const double deltaTime) {
    
}

void BattleScene::render(SDLContext& context) {
    SDL_RenderCopy(context.getRenderer(), background, NULL, &BackgroundPos);
    deckButton->render(context);
    quitButton->render(context);
    for(unique_ptr<Card>& card : cards) {
        card->render(context);
    }
    fort->render(context);
}

bool BattleScene::handleEvent(const SDL_Event& event) {
    bool eventHandled = false;
    
    if(event.type == SDL_QUIT) {
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
                //deleteCard(i);
                break;
            }
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
        Vec2<short> cardPos;
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
    
    Vec2<short> cardPos;
    cardPos.y = HandPosition.y;
    for(size_t i = 0; i < cards.size(); ++i) {
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
    Card1ThumbTexID = textureManager.load(Card1Thumbnail);
    Card1PreviewTexID = textureManager.load(Card1Preview);
    Card2ThumbTexID = textureManager.load(Card2Thumbnail);
    Card2PreviewTexID = textureManager.load(Card2Preview);
    Card3ThumbTexID = textureManager.load(Card3Thumbnail);
    Card3PreviewTexID = textureManager.load(Card3Preview);
    RoomHighlightTexID = textureManager.load(RoomHighlight);
    Room1TexID = textureManager.load(Room1Image);
    Room2TexID = textureManager.load(Room2Image);
    Room3TexID = textureManager.load(Room3Image);
}

void BattleScene::loadCards() {
    const ExitType allExits = static_cast<ExitType>(ET_Up | ET_Down | ET_Left | ET_Right);
    
    SDL_Texture* room1Tex = textureManager.getByID(Room1TexID);
    Room room1(allExits, 1, room1Tex);
    RoomCard* card1 = new RoomCard(1, cardThumbnails[0], cardPreviews[0], room1, *fort);
    cardFactory.addCard(card1);
    cardIDs.push_back(1);
    
    SDL_Texture* room2Tex = textureManager.getByID(Room2TexID);
    Room room2(allExits, 2, room2Tex);
    RoomCard* card2 = new RoomCard(2, cardThumbnails[1], cardPreviews[1], room2, *fort);
    cardFactory.addCard(card2);
    cardIDs.push_back(2);
    
    SDL_Texture* room3Tex = textureManager.getByID(Room3TexID);
    Room room3(allExits, 3, room3Tex);
    RoomCard* card3 = new RoomCard(3, cardThumbnails[2], cardPreviews[2], room3, *fort);
    cardFactory.addCard(card3);
    cardIDs.push_back(3);
}

void BattleScene::cleanup() {
    cards.clear();
}
