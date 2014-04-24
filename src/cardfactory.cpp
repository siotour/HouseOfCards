#include"../include/cardfactory.hpp"
#include"../include/card.hpp"
#include<avl/include/exceptions.hpp>
#include<map>
#include<memory>
#include<utility>

using namespace std;
using namespace avl;


Card* cloneCard(const Card& original) {
    Card* clone = nullptr;
    
    if(original.getType() == CT_Room) {
        const RoomCard& roomCard = dynamic_cast<const RoomCard&>(original);
        clone = new RoomCard(roomCard);
    } else {
        // Unknown card type
        throw InvalidArgumentException(__FILE__, __LINE__, "original", "Unrecognized card type.");
    }
    
    if(clone == nullptr) {
        throw OutOfMemoryException(__FILE__, __LINE__);
    }
    
    return clone;
}


CardFactory::CardFactory() {
    
}

void CardFactory::addCard(Card* const card) {
    if(card == nullptr) {
        throw NullPointerException(__FILE__, __LINE__);
    }
    
    auto result = cards.insert(make_pair(card->getID(), unique_ptr<Card>(card)));
    if(result.second == false) {
        // Card ID already being used
        throw InvalidArgumentException(__FILE__, __LINE__, "card", "A card with this id has already been added.");
    }
}

Card* CardFactory::makeCard(const CardID id) {
    auto iter = cards.find(id);
    
    if(iter == cards.end()) {
        // Card does not exist
        throw InvalidArgumentException(__FILE__, __LINE__, "id", "The requested card doesn't exist.");
    }
    
    Card* const clone = cloneCard(*(iter->second.get()));
    return clone;
}