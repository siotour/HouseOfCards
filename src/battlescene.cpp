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

const SDL_Rect BackgroundPos = {0, 0, 1024, 768};

const SDL_Rect DeckButtonPos = {760, 600, 110, 154};
const SDL_Rect QuitButtonPos = {910, 660, 70, 70};

const unsigned int MaxCards = 6;
// Hand of cards top/left corner
const Vec2<short> HandPosition = {10, 600};
const short CardSpacing = 120;



BattleScene::BattleScene(SDLContext& context)
: sceneDone(false)
{
    loadTextures(context);
    
    deckButton.reset(new Button(DeckButtonPos, context, DeckButtonInactive, DeckButtonActive));
    quitButton.reset(new Button(QuitButtonPos, context, QuitButtonInactive, QuitButtonActive));
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

void BattleScene::loadTextures(SDLContext& context) {
    background = IMG_LoadTexture(context.getRenderer(), BackgroundImage.c_str());
    if(background == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardThumbnails[0] = IMG_LoadTexture(context.getRenderer(), Card1Thumbnail.c_str());
    if(cardThumbnails[0] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardPreviews[0] = IMG_LoadTexture(context.getRenderer(), Card1Preview.c_str());
    if(cardPreviews[0] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardThumbnails[1] = IMG_LoadTexture(context.getRenderer(), Card2Thumbnail.c_str());
    if(cardThumbnails[1] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardPreviews[1] = IMG_LoadTexture(context.getRenderer(), Card2Preview.c_str());
    if(cardPreviews[1] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardThumbnails[2] = IMG_LoadTexture(context.getRenderer(), Card3Thumbnail.c_str());
    if(cardThumbnails[2] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    
    cardPreviews[2] = IMG_LoadTexture(context.getRenderer(), Card3Preview.c_str());
    if(cardPreviews[2] == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
}

void BattleScene::cleanup() {
    cards.clear();
    
    if(background != nullptr) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }
    for(size_t i = 0; i < 3; ++i) {
        if(cardThumbnails[i] != nullptr) {
            SDL_DestroyTexture(cardThumbnails[i]);
            cardThumbnails[i] = nullptr;
        }
        if(cardPreviews[i] != nullptr) {
            SDL_DestroyTexture(cardPreviews[i]);
            cardPreviews[i] = nullptr;
        }
    }
}
