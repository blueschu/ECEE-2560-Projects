//
// Created by chand on 10/13/2020.
//

#ifndef ASSIGNMENT_2_CARD_H
#define ASSIGNMENT_2_CARD_H

class Card
{
public:
    enum class Suit {Begin, HEART = Begin, DIAMOND, CLUB, SPADE, End};
    enum class Value {Begin, A = Begin, _2, _3, _4, _5, _6, _7, _8, _9, _10, J, Q, K, End};

    Card(Suit suit, Value value); //parameterized constructor
    ~Card(); //destructor

    Suit getSuit(); // function to return the suit of the card
    void setSuit(Suit suit);
    Value getValue(); //function to return the value of the card
    void setValue(Value value);

private:
    Value cardValue; //creates a card variable for the value
    Suit cardSuit; //creates a card variable for the suit
};

#endif //ASSIGNMENT_2_CARD_H
