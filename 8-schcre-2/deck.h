//
// Created by chand on 10/13/2020.
//

#ifndef ASSIGNMENT_2_DECK_H
#define ASSIGNMENT_2_DECK_H

#include <vector>
#include "card.h"

// Class Deck

using namespace std;

class Deck
{
public:
    Deck(); //default constructor for class CardDeck
    Card GetCard(Card& card);
    void newCard(Card& card);
    void shuffle();
    vector<Card> drawCards(short drawnCards);
    void CreateDeck();
    ~Deck();

private:
    vector <Card>* pCards;
    short maxCards;
    short numCards;
    Card card;
};

#endif //ASSIGNMENT_2_DECK_H
