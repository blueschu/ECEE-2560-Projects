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
 *  - https://en.cppreference.com/w/cpp/header/random
 */

#ifndef ECEE_2560_PROJECTS_CODE_H
#define ECEE_2560_PROJECTS_CODE_H

#include <algorithm>        // for std::generate
#include <cstdint>          // for fixed width integers
#include <functional>       // for std::bind
#include <iosfwd>           // for I/O declarations, full iostream header not required.
#include <stdexcept>        // for std::invalid_argument
#include <random>           // for random number generation
#include <vector>           // for std::vector

class Code {
  public:
    // Integral type to be used for representing code digits.
    using Digit = std::uint8_t;

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
        std::size_t digit_range,
        R entropy_source = R(default_random_seed())
    ) : m_digits(digit_count)
    {
        // Random distribution for integers in the interval [0, digit_range).
        const std::uniform_int_distribution<Digit> distribution(0, static_cast<Digit>(digit_range - 1));
        // Convenience function object for calling `distribution(entropy_source)`.
        const auto digit_generator = std::bind(distribution, entropy_source);

        // Fill `m_digits` using consecutive calls to `digit_generator`.
        std::generate(std::begin(m_digits), std::end(m_digits), digit_generator);
    }

    /**
     * Counts the number of digits that match in both value and position between
     * this code and the given guess.
     *
     * Marked as [[nodiscard]] per linter recommendation.
     *
     * @param guess Guess for the secret code digits.
     * @return The number of correct digits in the guess.
     */
    [[nodiscard]]
    std::size_t checkCorrect(const Code& guess) const;

    /**
     * Counts the number of digits that match in value but do not match in
     * position between this code and the given guess.
     *
     * @param guess Guess for the secret code digits.
     * @return The number of incorrect digits in the guess.
     */
    [[nodiscard]]
    std::size_t checkIncorrect(const Code& guess) const;

    // Output stream operator overlaod.
    friend std::ostream& operator<<(std::ostream& out, const Code&);

  private:

    /// The digits of this secret code.
    std::vector<Digit> m_digits;

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
