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
uniform_int_distribution<int> dist(0, 2);

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
                deleteCard(i);
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
        int cardIndex = dist(rng);
        cards.emplace_back(new Card(cardPos, cardThumbnails[cardIndex], cardPreviews[cardIndex]));
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
}

void BattleScene::cleanup() {
    cards.clear();
}
