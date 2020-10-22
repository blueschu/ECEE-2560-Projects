/**
 * Card class for Project 2a.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *
 *  - [P0627r0] http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0627r0.pdf
 *  - [1]: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */

#include "card.h"

#include <ostream>          // output stream definitions

using namespace std::string_view_literals;

// Using anonymous namespace to give symbols internal linkage.
namespace {
constexpr char suit_to_char(Card::Suit suit)
{
    switch (suit) {
        case Card::Suit::Club: return 'C';
        case Card::Suit::Diamond: return 'D';
        case Card::Suit::Heart: return 'H';
        case Card::Suit::Spade: return 'S';
    }

    // Signal to the compiler that this branch is impossible [P0627r0,1].
    __builtin_unreachable();
}

constexpr char rank_to_char(Card::Rank rank)
{
    switch (rank) {
        case Card::Rank::Ace: return 'A';
        case Card::Rank::R2: return '2';
        case Card::Rank::R3: return '3';
        case Card::Rank::R4: return '4';
        case Card::Rank::R5: return '5';
        case Card::Rank::R6: return '6';
        case Card::Rank::R7: return '7';
        case Card::Rank::R8: return '8';
        case Card::Rank::R9: return '9';
        case Card::Rank::R10: return 'T';
        case Card::Rank::Jack: return 'J';
        case Card::Rank::Queen: return 'Q';
        case Card::Rank::King: return 'K';
    }

    // Signal to the compiler that this branch is impossible [P0627r0,1].
    __builtin_unreachable();
}

} // end namespace

std::ostream& operator<<(std::ostream& out, Card::Suit suit)
{
    out << suit_to_char(suit);
    return out;
}

std::ostream& operator<<(std::ostream& out, Card::Rank rank)
{
    out << rank_to_char(rank);
    return out;
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    out << card.m_rank << card.m_suit;
    return out;
}
