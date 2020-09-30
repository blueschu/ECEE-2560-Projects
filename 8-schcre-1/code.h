/**
 * Code class for Project 1.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 * References
 * ==========
 *
 *  [1]: https://en.cppreference.com/w/cpp/header/random
 *  [2]: https://en.cppreference.com/w/cpp/numeric/random
 */

#ifndef ECEE_2560_PROJECTS_CODE_H
#define ECEE_2560_PROJECTS_CODE_H

#include <algorithm>        // for std::generate
#include <cstdint>          // for fixed width integers
#include <functional>       // for std::bind
#include <iosfwd>           // for I/O declarations, full iostream header not required.
#include <limits>           // for std::numeric_limits
#include <stdexcept>        // for std::invalid_argument
#include <string>           // for std::to_string
#include <random>           // for random number generation
#include <utility>          // for std::tie
#include <vector>           // for std::vector

/**
 * A response to a guess during a mastermind game.
 *
 * Usually, this class would be implemented as a POD structure. However,
 * this project required that it be implemented as a non-trivial class with
 * getters, setters, and a user-defined constructor.
 */
class GuessResponse {
  public:
    /// Integral type representing a count.
    using Count = unsigned int;

  private:
    /**
     * The number of digits in the guess that match the code in value and position.
     */
    Count m_correct_count;
    /**
     * The number of digits in the guess that match the code in value, but not
     * in position.
     */
    Count m_incorrect_count;

  public:
    /**
     * Constructs a GuessResponse with the given correct and incorrect counts.
     *
     * Note: it may be preferable to replace this constructor with aggregate
     * initialization in a more general design.
     */
    GuessResponse(Count correct, Count incorrect)
        : m_correct_count{correct}, m_incorrect_count{incorrect} {};

    /// Returns the number of correct digits.
    [[nodiscard]] Count get_correct() const { return m_correct_count; }

    /// Sets the number of correct digit to the given value.
    void set_correct(Count value) { m_correct_count = value; }

    /// Returns the number of incorrect digits.
    [[nodiscard]] Count get_incorrect() const { return m_incorrect_count; }

    /// Sets the number of incorrect digits to the given value.
    void set_incorrect(Count value) { m_incorrect_count = value; }

    // Comparison operator overload. Implicitly inline.
    friend bool operator==(const GuessResponse& lhs, const GuessResponse& rhs)
    {
        return std::tie(lhs.m_correct_count, lhs.m_incorrect_count)
            == std::tie(rhs.m_correct_count, rhs.m_incorrect_count);
    }
};

// Output stream operator overload.
std::ostream& operator<<(std::ostream& out, const GuessResponse& guess_response);

/**
 * Exception class thrown when `Code::check_guess(Code)` is called on Code
 * instances with an unequal number of digits.
 */
class MismatchedCodeLengthError : public std::invalid_argument {
    // Use parent class constructor.
    using std::invalid_argument::invalid_argument;
};

/**
 * A "secret code" for the MasterMind game.
 */
class Code {
  public:
    // Integral type to be used for representing code digits.
    using Digit = std::uint8_t;

  private:
    /// The digits of this secret code.
    std::vector<Digit> m_digits;

  public:

    /**
     * Initialize a Code instance with the given digits.
     *
     * @param list Digit sequence.
     */
    Code(std::initializer_list<Digit> list) : m_digits{list} {}

    /**
     * Generates a random secret code with `digit_count` digits each ranging
     * from 0 to `digit_range - 1`, optionally with a custom random engine.
     *
     * @tparam R Random number generator.
     * @param digit_count Number of digits to include in the code.
     * @param digit_range Upper bound of code digits, not inclusive. Must not
     *                    exceed the maximum value representable by Digit, plus
     *                    one.
     * @param entropy_source Random number generator.
     * @throws invalid_argument if the given digit_range exceeds the maximum
     *                    value representable by Digit, plus
     *                    one.
     */
    template<typename R = std::default_random_engine>
    Code(
        std::size_t digit_count,
        unsigned int digit_range,
        R entropy_source = R(default_random_seed())
    ) : m_digits(digit_count)
    {
        using namespace std::string_literals;
        // The maximum value representable by a digit, plus one.
        constexpr auto max_radix{
            // Promote ULL to ensure that we don't overflow the Digit type.
            std::numeric_limits<Digit>::max() + 1ull
        };

        // Make sure we can represent the interval [0, max_digit) using the
        // configured Digit type.
        if (digit_range > max_radix) {
            throw std::invalid_argument(
                "digit range cannot exceed the boundaries of the digit type "
                "(max radix is "s + std::to_string(max_radix) + ")"
            );
        }

        // Random distribution for integers in the interval [0, digit_range).
        const std::uniform_int_distribution<Digit> distribution(0, static_cast<Digit>(digit_range - 1));
        // Convenience function object for calling `distribution(entropy_source)`.
        const auto digit_generator = std::bind(distribution, entropy_source);

        // Fill `m_digits` using consecutive calls to `digit_generator`.
        std::generate(std::begin(m_digits), std::end(m_digits), digit_generator);
    }

    /**
     * Constructs a `Code` instance with the given digits.
     *
     * @param digits Code digits.
     */
    explicit Code(std::vector<Digit> digits) : m_digits(std::move(digits)) {}

    // Output stream operator overload.
    friend std::ostream& operator<<(std::ostream& out, const Code&);

    /**
     * Compares the given guess against this codes digits and produces a guess
     * result containing the number of "correct" and "incorrect" digits,
     * according to the Mastermind game rules.
     *
     * Marked with a [[nodiscard]] attribute since calling this member function
     * without reading the return value would be nonsensical.
     *
     * @param guess Guess for the secret code digits.
     * @throws MismatchedCodeLengthError if this code and the given guess
     *         have unequal lengths.
     * @return Guess result.
     */
    [[nodiscard]]
    GuessResponse check_guess(const Code& guess) const
    {
        return GuessResponse(check_correct(guess), check_incorrect(guess));
    }

  private:

    /**
     * Counts the number of digits that match in both value and position between
     * this code and the given guess.
     *
     * Marked with a [[nodiscard]] attribute since calling this member function
     * without reading the return value would be nonsensical.
     *
     * Runs in O(N) times and O(1) space.
     *
     * @param guess Guess for the secret code digits.
     * @throws MismatchedCodeLengthError if this code and the given guess
     *         have unequal lengths.
     * @return The number of correct digits in the guess.
     */
    [[nodiscard]]
    GuessResponse::Count check_correct(const Code& guess) const;

    /**
     * Counts the number of digits that match in value but do not match in
     * position between this code and the given guess.
     *
     * Marked with a [[nodiscard]] attribute since calling this member function
     * without reading the return value would be nonsensical.
     *
     * Runs in O(N log N) time and O(N) space.
     *
     * @param guess Guess for the secret code digits.
     * @throws MismatchedCodeLengthError if this code and the given guess
     *         have unequal lengths.
     * @return The number of incorrect digits in the guess.
     */
    [[nodiscard]]
    GuessResponse::Count check_incorrect(const Code& guess) const;

    /**
     * Helper function for producing a random PRNG seed using an available
     * hardware device.
     *
     * @return PRNG seed.
     */
    [[nodiscard]]
    static std::random_device::result_type default_random_seed()
    {
        // Handle for random seed generator using available hardware devices.
        // This technique for acquiring PRNG seeds is more secure than using
        // e.g. the current system time. This variable is given static duration
        // so that only one random device will be produced for this class.
        static std::random_device random_device{};
        // Return a freshly generated PRNG seed.
        return random_device();
    }

}; // class Code

#endif //ECEE_2560_PROJECTS_CODE_H
