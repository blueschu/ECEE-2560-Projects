/**
 * Mastermind game class declarations.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 * References
 * ==========
 *
 *  [1]: https://en.cppreference.com/w/cpp/language/inline
 *
 */

#ifndef ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
#define ECEE_2560_PROJECTS_MASTER_MIND_GAME_H

#define EECE_USE_OVERLOADS

#include "code.h"

#include <functional>       // for std::function

/**
 * A game of MasterMind with a fixed secret code. Implemented for part b.
 */
class MasterMindGame {

    // Note: constexpr static member variables are implicitly inline since C++17 [1].

    /// The default number of digits in a secret code.
    constexpr static std::size_t DEFAULT_CODE_SIZE{5};

    /// The default radix for secret code digits.
    constexpr static unsigned int DEFAULT_DIGIT_RADIX{10};

    /// The number of incorrect guesses a player can make before losing the game.
    constexpr static unsigned int MAX_GUESSES{10};

    /// The number of digits in this game's secret code.
    const std::size_t m_code_size;

    /// This game's secret code.
    const Code m_secret_code;

  public:
    /**
     * Callable type for generating guesses during a mastermind game.
     *
     * This callable accepts a single argument representing the number of
     * digits in the game's secret code.
     */
    using GuessGenerator = std::function<Code(std::size_t)>;

    /**
     * Callable type for displaying the number of guesses remaining and the
     * result of a guess during a mastermind game.
     *
     * This callable accepts two arguments representing the number of guess
     * remaining and the GuessResponse from the last guess, respectively.
     */
    using ResponseCallback = std::function<void(int, GuessResponse)>;

#ifdef EECE_USE_OVERLOADS
    // Use overloaded constructors to provide default constructor arguments.

    /**
     * Creates a mastermind game with default digit count and digit range.
     */
    MasterMindGame() : MasterMindGame(DEFAULT_CODE_SIZE, DEFAULT_DIGIT_RADIX) {}

    /**
     * Creates a mastermind game with an n-digit secret code in radix r, where
     * n = `code_size` and r = `digit-range`.
     *
     * @param code_size The number of digit in the secret code.
     * @param digit_range The radix of the secret code digits.
     */
    MasterMindGame(std::size_t code_size, unsigned int digit_range)
        : m_code_size{code_size}, m_secret_code(code_size, digit_range) {}

#else // Use default arguments instead of overloaded constructors.

    /**
     * Creates a mastermind game with an n-digit secret code in radix r, where
     * n = `code_size` and r = `digit-range`.
     *
     * @param code_size The number of digit in the secret code.
     * @param digit_range The radix of the secret code digits.
     */
    explicit MasterMindGame(
        std::size_t code_size = DEFAULT_CODE_SIZE,
        unsigned int digit_range = DEFAULT_DIGIT_RADIX
    ) : m_code_size{code_size}, m_secret_code(code_size, digit_range) {}

#endif

    /**
     * Returns this game's secret code.
     *
     * @return Ths game's secret code.
     */
    [[nodiscard]]
    const Code& get_code() const noexcept { return m_secret_code; }

    /**
     * Runs a game of mastermind using this game's secret code.
     *
     * This function accepts a `guess_generator` function object for producing
     * user guesses during the same.
     *
     * This function implements the "playGame()" function from the project
     * instructions.
     *
     * @param guess_generator Callable for generating user guesses.
     * @param response_callback Callback for displaying use results to the user.
     * @return true if the user one the game, false if they lost.
     */
    bool run_game(
        const GuessGenerator& guess_generator,
        const ResponseCallback& response_callback
    ) const;

  private:
    /**
     * Returns true if the given guess response indicates that the user won
     * the game.
     *
     * This function implements the "isSolved()" function from the project
     * instructions.
     *
     * @param guess_response Response to the user's guess.
     * @return True if the response indicates that the user won the game.
     */
    [[nodiscard]]
    bool check_solution(const GuessResponse& guess_response) const noexcept
    {
        return guess_response.correct_count == m_code_size;
    }

    /**
     * Returns the response to the given user guess for this MasterMind game.
     *
     * This function implement the "getResponse()" function from the project
     * instructions.
     *
     * @param guess Guess for the secret code digits.
     * @return Guess result.
     */
    [[nodiscard]]
    GuessResponse compute_guess_response(const Code& guess) const
    {
        return m_secret_code.check_guess(guess);
    }

    /**
     * Generates a user guess with the provided guess generator.
     *
     * This function implement the "humanGuess()" function from the project
     * instructions.
     *
     * @param guess_generator Callable for generating user guesses.
     * @return User guess for the secret code.
     */
    [[nodiscard]]
    Code generate_user_guess(const GuessGenerator& guess_generator) const
    {
        return guess_generator(m_code_size);
    }

};

#endif //ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
