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
 *  [3]: https://en.cppreference.com/w/cpp/language/aggregate_initialization
 *  [4]: https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special
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
 * POD structure representing a response to a guess during a mastermind game.
 *
 * We intentionally do not make `correct_count` and `incorrect_count` private
 * since this class is not responsible for upholding any invariants.
 *
 * This class is an aggregate class, so no user-defined constructors are
 * provided.
 */
struct GuessResponse {
    /// Integral type representing a count.
    using Count = unsigned int;

    /**
     * The number of digits in the guess that match the code in value and position.
     */
    Count correct_count;
    /**
     * The number of digits in the guess that match the code in value, but not
     * in position.
     */
    Count incorrect_count;

    // Constructor not defined since this class is designed to be an aggregate.
    GuessResponse() = delete;
};

static_assert(
    std::is_aggregate_v<GuessResponse>,
    "GuessResponse is expected to be an aggregate data type."
);

static_assert(
    std::is_pod_v<GuessResponse>,
    "GuessResponse is expected to be a plain-old-data type."
);

// Comparison operator overload.
inline bool operator==(const GuessResponse& lhs, const GuessResponse& rhs)
{
    return std::tie(lhs.correct_count, lhs.incorrect_count)
        == std::tie(rhs.correct_count, rhs.incorrect_count);
}

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
     * @param digit_range Upper bound of code digits, not inclusive.
     * @param entropy_source Random number generator.
     */
    template<typename R = std::default_random_engine>
    Code(
        std::size_t digit_count,
        unsigned int digit_range,
        R entropy_source = R(default_random_seed())
    ) : m_digits(digit_count)
    {
        using namespace std::string_literals;
        constexpr auto max_digit = std::numeric_limits<Digit>::max();

        // Make sure we can represent the interval [0, max_digit) using the
        // configured Digit type.
        if (digit_range - 1 > max_digit) {
            throw std::invalid_argument(
                "digit range cannot exceed the boundaries of the digit type "
                "(max value is "s + std::to_string(max_digit) + ")"
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
     * Compares the given guess against this codes digits and produceds a guess
     * result containing the number of "correct" and "incorrect" digits,
     * according the the Mastermind game rules.
     *
     * Marked with a [[nodiscard]] attribute since calling this member function
     * without reading the return value would be nonsensical.
     *
     * @param guess Guess for the secret code digits.
     * @return Guess result.
     */
    [[nodiscard]]
    GuessResponse check_guess(const Code& guess) const
    {
        return {check_correct(guess), check_incorrect(guess)};
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
