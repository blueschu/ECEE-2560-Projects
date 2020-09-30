/**
 * Card class for Project 2a.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 */

#ifndef EECE_2560_PROJECTS_CARD_H
#define EECE_2560_PROJECTS_CARD_H

#include <tuple>            // for std::tie

class Card {

  public:
    enum class Suit {
        Club, Diamond, Heart, Spade
    };

    enum class Rank {
        R2, R3, R4, R5, R6, R7, R8, R9, R10, Jack, Queen, King, Ace
    };

    Card(Suit suit, Rank rank) : m_suit{suit}, m_rank{rank} {}

    bool is_face()
    {
        return m_rank == Rank::Jack || m_rank == Rank::Queen || m_rank == Rank::King;
    }

    [[nodiscard]] Suit get_suit() const { return m_suit; }

    void set_suit(Suit suit) { m_suit = suit; }

    [[nodiscard]] Rank get_rank() const { return m_rank; }

    void set_rank(Rank rank) { m_rank = rank; }

    bool operator==(const Card& rhs) const
    {
        return std::tie(m_suit, m_rank) == std::tie(rhs.m_suit, rhs.m_rank);
    }

    bool operator!=(const Card& rhs) const
    {
        return !(rhs == *this);
    }

    friend std::ostream& operator<<(std::ostream& os, const Card& card);

  private:

    Suit m_suit;
    Rank m_rank;

};

std::ostream& operator<<(std::ostream& out, Card::Suit suit);

std::ostream& operator<<(std::ostream& out, Card::Rank rank);

#endif //EECE_2560_PROJECTS_CARD_H
