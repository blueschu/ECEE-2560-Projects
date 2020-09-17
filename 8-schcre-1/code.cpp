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
 *  - https://en.cppreference.com/w/cpp/algorithm/set_intersection
 *
 */

#include "code.h"

#include <iostream>

std::size_t Code::checkCorrect(const Code& guess) const
{
    if (m_digits.size() != guess.m_digits.size()) {
        throw std::invalid_argument("cannot compare Code instances of unequal lengths");
    }

    std::size_t match_count{0};

    auto right_it = std::begin(guess.m_digits);

    for (const Digit left_digit : m_digits) {
        if (left_digit == *right_it) {
            ++match_count;
        }
        ++right_it;
    }

    return match_count;
}

std::size_t Code::checkIncorrect(const Code& guess) const
{
    // Sequence of digits from this->m_digits that do not match guess.m_digits
    // in value and position.
    std::vector<Digit> differing_digits_left{};
    // Sequence of digits from guess.m_digits that do not match this->m_digits
    // in value and position.
    std::vector<Digit> differing_digits_right{};

    auto right_it = std::begin(guess.m_digits);

    // Extract all digits that do not match in value and position from both codes.
    for (const Digit left_digit : m_digits) {
        if (left_digit != *right_it) {
            differing_digits_left.push_back(left_digit);
            differing_digits_right.push_back(*right_it);
        }
        ++right_it;
    }

    // Sort the differing digits to that they can be treated as sets.
    std::sort(std::begin(differing_digits_left), std::end(differing_digits_left));
    std::sort(std::begin(differing_digits_right), std::end(differing_digits_right));

    std::vector<Digit> incorrect_digits{};

    // Extract the digits that are present in both sequences with repeats
    // treated as unique values.
    std::set_intersection(
        std::begin(differing_digits_left), std::end(differing_digits_left),
        std::begin(differing_digits_right), std::end(differing_digits_right),
        std::inserter(incorrect_digits, std::begin(incorrect_digits))
    );

    return incorrect_digits.size();

}

std::ostream& operator<<(std::ostream& out, const Code& code)
{
    out << "[ ";
    for (Code::Digit digit : code.m_digits) {
        // Make sure that digits are not displayed as `char`.
        out << static_cast<int>(digit) << ' ';
    }
    out << ']';

    return out;
}
