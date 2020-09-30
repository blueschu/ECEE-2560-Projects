/**
 * Card class for Project 2a.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *
 *  - [P0627r0] http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0627r0.pdf
 */

#include "card.h"

#include <ostream>
#include <string_view>

using namespace std::string_view_literals;

// Using anonymous namespace to give symbols internal linkage.
namespace {
constexpr std::string_view suite_to_string(Card::Suit suit)
{
    switch (suit) {
        case Card::Suit::Club: return "C";
        case Card::Suit::Diamond: return "D"sv;
        case Card::Suit::Heart: return "H"sv;
        case Card::Suit::Spade: return "S"sv;
    }

    // Signal the compiler that this branch is impossible [P0627r0].
    __builtin_unreachable();
}

constexpr std::string_view rank_to_string(Card::Rank rank)
{
    switch (rank) {
        case Card::Rank::R2: return "2"sv;
        case Card::Rank::R3: return "3"sv;
        case Card::Rank::R4: return "4"sv;
        case Card::Rank::R5: return "5"sv;
        case Card::Rank::R6: return "6"sv;
        case Card::Rank::R7: return "7"sv;
        case Card::Rank::R8: return "8"sv;
        case Card::Rank::R9: return "9"sv;
        case Card::Rank::R10: return "10"sv;
        case Card::Rank::Jack: return "J"sv;
        case Card::Rank::Queen: return "Q"sv;
        case Card::Rank::King: return "K"sv;
        case Card::Rank::Ace: return "A"sv;
    }

    // Signal the compiler that this branch is impossible [P0627r0].
    __builtin_unreachable();
}

} // end namespace

std::ostream& operator<<(std::ostream& out, Card::Suit suit)
{
    out << suite_to_string(suit);
    return out;
}

std::ostream& operator<<(std::ostream& out, Card::Rank rank)
{
    out << rank_to_string(rank);
    return out;
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    out << card.m_rank << card.m_suit;
    return out;
}
