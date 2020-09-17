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
 *  - https://en.cppreference.com/w/cpp/iterator/inserter
 *
 */

#include "code.h"

#include <iostream>

std::size_t Code::checkCorrect(const Code& guess) const
{
    if (m_digits.size() != guess.m_digits.size()) {
        throw std::invalid_argument("cannot compare Code instances of unequal lengths");
    }

    // The number of digits that match in both value and position.
    std::size_t match_count{0};
    // Iterator into the guess code.
    auto right_it = std::begin(guess.m_digits);

    // Simultaneously iterate over the digits in this code and in the guess
    // code. Count the number of position where the digits mathc.
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
    if (m_digits.size() != guess.m_digits.size()) {
        throw std::invalid_argument("cannot compare Code instances of unequal lengths");
    }

    // Sequence of digits from this code that do not match the guess digits
    // in both value and position. Relative ordering of digits is maintained.
    std::vector<Digit> differing_digits_left{};
    // Sequence of digits from the guess that do not match this code's digits
    // in both value and position. Relative ordering of digits is maintained.
    std::vector<Digit> differing_digits_right{};

    // Iterator into the guess code.
    auto right_it = std::begin(guess.m_digits);

    // Simultaneously iterate over the digits in this code and in the guess
    // code. Whenever the digits in this code and the guess code do not agree,
    // copy the digits into the "differing digits" vectors.
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

    // Extract the digits that are present in both "differing digits" sequences,
    // with repeats treated as unique values. These are precisely the values
    // that appear in both sequences, but which do not appear in the same
    // positions.
    std::set_intersection(
        std::begin(differing_digits_left),
        std::end(differing_digits_left),
        std::begin(differing_digits_right),
        std::end(differing_digits_right),
        std::inserter(incorrect_digits, std::begin(incorrect_digits))
    );

    return incorrect_digits.size();

}

std::ostream& operator<<(std::ostream& out, const Code& code)
{
    out << "[ ";
    for (Code::Digit digit : code.m_digits) {
        // Make sure that digits are not interpreted as encoded characters,
        // since we use an integral type with the same size as
        out << static_cast<int>(digit) << ' ';
    }
    out << ']';

    return out;
}
