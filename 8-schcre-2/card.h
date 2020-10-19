/**
 * EECE 2560 Project 2 - Card class declarations.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *  [1] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef EECE_2560_PROJECTS_CARD_H
#define EECE_2560_PROJECTS_CARD_H

#include <array>            // for std::array
#include <tuple>            // for std::tie

/**
 * A playing card with a suit and rank.
 */
class Card {

  public:
    /// The possible ranks for a playing card.
    enum class Rank {
        Ace, R2, R3, R4, R5, R6, R7, R8, R9, R10, Jack, Queen, King
    };

    /// The possible suits for a playing card.
    enum class Suit {
        Club, Diamond, Heart, Spade
    };

    /// All possible playing car ranks
    constexpr static auto ALL_RANKS{
        std::array{
            Rank::Ace, Rank::R2, Rank::R3, Rank::R4, Rank::R5,
            Rank::R6, Rank::R7, Rank::R8, Rank::R9, Rank::R10,
            Rank::Jack, Rank::Queen, Rank::King
        }
    };

    /// All possible playing card suits.
    constexpr static auto ALL_SUITS{
        std::array{Suit::Club, Suit::Diamond, Suit::Heart, Suit::Spade}
    };

  private:
    /// The rank of this card.
    Rank m_rank;

    /// The suit of this card.
    Suit m_suit;

  public:

    /*
     * Creates a playing card with the given suit and rank.
     *
     * Making this constructor constexpr allows cards to be used as literals.
     *
     * This constructor could be omitted if we made Card an aggregate.
     */
    constexpr Card(Rank rank, Suit suit) : m_rank{rank}, m_suit{suit} {}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "modernize-use-equals-default"
    // Disable clang-tiddy linter diagnostics for trivial copy ctor.

    // User-defined copy constructor per project instructions.
    Card(const Card& rhs) noexcept: m_rank{rhs.m_rank}, m_suit{rhs.m_suit} {} // = default

    // User-defined copy assignment per project instructions.
    Card& operator=(const Card& rhs) noexcept // = default
    {
        std::tie(m_rank, m_suit) = std::tie(rhs.m_rank, rhs.m_suit);
        return *this;
    }

    // Since we defined copy constructor/assignment operator, we need to explicitly
    // re-enable move the constructor [1, C.21].
    Card(Card&& rhs) = default;

    // Since we defined copy constructor/assignment operator, we need to explicitly
    // re-enable move the assignment operator [1, C.21].
    Card& operator=(Card&& rhs) = default;

#pragma clang diagnostic pop

    /// Returns true if this card is a face card (Jack, Queen, or King).
    [[nodiscard]]
    bool is_face() const
    {
        return m_rank == Rank::Jack || m_rank == Rank::Queen || m_rank == Rank::King;
    }

    /*
     * Required getters and setters from project instructions.
     */

    /// Returns the suit of this card.
    [[nodiscard]] Suit get_suit() const { return m_suit; }

    /// Sets the suit of this card to the given value.
    void set_suit(Suit suit) { m_suit = suit; }

    /// Returns the rank of this card.
    [[nodiscard]] Rank get_rank() const { return m_rank; }

    /// Sets the rank of this card to the given value.
    void set_rank(Rank rank) { m_rank = rank; }

    /*
     * Comparison operator overloads.
     */

#if __cplusplus < 202002L
    bool operator==(const Card& rhs) const
    {
        return std::tie(m_rank, m_suit) == std::tie(rhs.m_rank, rhs.m_suit);
    }

    bool operator<(const Card& rhs) const
    {
        return std::tie(m_rank, m_suit) < std::tie(rhs.m_rank, m_suit);
    }

    bool operator!=(const Card& rhs) const { return !(rhs == *this); }

    bool operator>(const Card& rhs) const { return rhs < *this; }

    bool operator<=(const Card& rhs) const { return !(rhs < *this); }

    bool operator>=(const Card& rhs) const { return !(*this < rhs); }
#else
    // If we're compiling with C++20 support, we can use default comparisons.
    // The compiler will automatically generate all comparison operators.
    auto operator<=>(const Card&) const = default;
#endif

    // Output stream operator overload.
    friend std::ostream& operator<<(std::ostream& os, const Card& card);

}; // class Card


// Ensure that there are precisely 52 possible playing cards.
static_assert(Card::ALL_SUITS.size() * Card::ALL_RANKS.size() == 52);

// Output stream operator overload for suits.
std::ostream& operator<<(std::ostream& out, Card::Suit suit);

// Output stream operator overload for ranks.
std::ostream& operator<<(std::ostream& out, Card::Rank rank);

#endif //EECE_2560_PROJECTS_CARD_H
