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
    using Digit = std::uint8_t;

    explicit Code(std::vector<Digit> digits) : m_digits{std::move(digits)} {}

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

    [[nodiscard]]
    std::size_t checkCorrect(const Code& other) const;

    [[nodiscard]]
    std::size_t checkIncorrect(const Code& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Code&);

  private:

    std::vector<Digit> m_digits;

    /**
     * Helper function for producing a random PRNG seed using an available
     * hardware device.
     *
     * @return PRNG seed.
     */
    static std::random_device::result_type default_random_seed()
    {
        // Handle for random seed generation using available hardware devices.
        // This tecnique for acquiring PRNG seeds is more  secure than using
        // e.g. using the current system time. This variable is given static
        // duration so that only one random device will be produced for this
        // class.
        static std::random_device random_device{};
        // Return a freshly generated PRNG seed.
        return random_device();
    }

}; // class Code

#endif //ECEE_2560_PROJECTS_CODE_H
