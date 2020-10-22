/**
 * Project 2b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 */

#include <cmath>            // for std::ceil
#include <iomanip>          // for std::setw
#include <iostream>         // for I/O definitions

#include "eece2560_io.h"
#include "deck.h"

namespace {
/// Number of cards to include in a game of flip (max 52).
constexpr std::size_t FLIP_CARD_COUNT{24};

/// A playing card with a flipped state.
struct FlipCard {
    Card card;
    bool flipped;
};

/// Configuration settings for a game of "Flipped".
struct GameConfig {
    bool show_unflipped_cards;
    bool allow_repeat_flips;
};

/// Integral type used to represent a game score.
using Score = int;

Score play_flip_interactive(std::vector<FlipCard>& cards, const GameConfig& game_config);

void display_game_state(const std::vector<FlipCard>& cards, const GameConfig& game_config);

Card prompt_pick_card(std::vector<FlipCard>& cards, const GameConfig& game_config);

void update_score(Card card, Score& score);

} // end namespace

int main()
{

    GameConfig game_config{};

    game_config.show_unflipped_cards = eece2560::prompt_user<bool>(
        "Game config - Show unflipped cards? ",
        eece2560::bool_alpha_extractor
    );
    game_config.allow_repeat_flips = eece2560::prompt_user<bool>(
        "Game config - Allow repeat flips?   ",
        eece2560::bool_alpha_extractor
    );

    const auto show_unused_cards = eece2560::prompt_user<bool>(
        "Game config - Show unused cards?    ",
        eece2560::bool_alpha_extractor
    );

    const auto show_shuffling = eece2560::prompt_user<bool>(
        "Game config - Show shuffling?       ",
        eece2560::bool_alpha_extractor
    );

    // Generate the required linked list of shuffled playing cards.
    Deck deck{};
    if (show_shuffling) {
        std::cout << "Deck before shuffling: " << deck << '\n';
    }
    deck.shuffle();
    if (show_shuffling) {
        std::cout << "Deck after shuffling:  " << deck << '\n';
    }

    // Sequence of cards to be used during the flip game.
    std::vector<FlipCard> live_cards;

    // "Deal" the required number of cards from the deck.
    for (std::size_t i{0}; i < FLIP_CARD_COUNT; ++i) {
        auto card = deck.deal();
        if (card) {
            live_cards.push_back({*card, false});
        } else {
            std::cerr << "Ran out of cards while dealing - ending the game\n";
            return 1;
        }
    }

    if (show_unused_cards) {
        std::cout << "Remaining cards in the deck: " << deck << '\n';
    }

    std::cout << '\n';

    // Run the interactive game until completion.
    auto score = play_flip_interactive(live_cards, game_config);

    std::cout << "Your final score was " << score << "!\n";
    return 0;
}

namespace {
Score play_flip_interactive(std::vector<FlipCard>& cards, const GameConfig& game_config)
{
    // String to be printed around each round header.
    const static std::string header_padding = std::string(FLIP_CARD_COUNT * 3 / 2 - 5, '=');

    int round_counter{1};
    Score score{};

    while (true) {
        // Print the round header.
        std::cout << header_padding << " Round " << std::setw(2) << round_counter << ' ' << header_padding << '\n';

        // Display the current state of the game cards.
        display_game_state(cards, game_config);
        std::cout << '\n';

        // Prompt the user to flip a card.
        auto card = prompt_pick_card(cards, game_config);

        update_score(card, score);

        std::cout << "You flipped " << card
                  << ". Your new score is " << score << "\n\n";

        ++round_counter;

        if (eece2560::prompt_user<bool>(
            "Would you like to quit? ",
            eece2560::bool_alpha_extractor
        )) {
            break;
        }
    }

    return score;
}

void display_game_state(const std::vector<FlipCard>& cards, const GameConfig& game_config)
{
    // Print the line of card indices.
    for (std::size_t i{0}; i < cards.size(); ++i) {
        if (!game_config.allow_repeat_flips && cards[i].flipped) {
            std::cout << " **";
        } else {
            std::cout << ' ' << std::setw(2) << i;
        }
    }
    std::cout << '\n';

    // Print the line of cards.
    for (const auto card : cards) {
        if (game_config.show_unflipped_cards || card.flipped) {
            std::cout << ' ' << card.card;
        } else {
            std::cout << ' ' << "??";
        }
    }
    std::cout << '\n';
}

Card prompt_pick_card(std::vector<FlipCard>& cards, const GameConfig& game_config)
{
    while (true) {
        const auto user_selection = eece2560::prompt_user<std::size_t>(
            "Pick a card: ",
            eece2560::FromIntervalExtractor(FLIP_CARD_COUNT)
        );

        if (!game_config.allow_repeat_flips && cards[user_selection].flipped) {
            std::cout << "You can't flip that card again!\n";
            continue;
        }

        cards[user_selection].flipped = true;
        return cards[user_selection].card;
    }
}

void update_score(Card card, Score& score)
{
    if (card.get_rank() == Card::Rank::Ace) {
        score += 10;
    }

    if (card.is_face()) {
        score += 5;
    }

    if (card.get_rank() == Card::Rank::R7) {
        // Use explicit casts since we're compiling with -Wconversion.
        score = static_cast<Score>(
            std::ceil(static_cast<double>(score) / 2)
        );
    }

    if (card.is_low()) {
        score = 0;
    }

    if (card.get_suit() == Card::Suit::Heart) {
        score += 1;
    }

}
}

