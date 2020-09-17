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
 *  - https://en.cppreference.com/w/cpp/algorithm/set_difference
 *
 */

#include "code.h"

#include <iostream>

std::size_t Code::checkCorrect(const Code& other) const
{
    if (m_digits.size() != other.m_digits.size()) {
        throw std::invalid_argument("cannot compare Code instances of unequal lengths");
    }

    std::size_t match_count{0};

    auto right_it = std::begin(other.m_digits);

    for (const Digit left_digit : m_digits) {
        if (left_digit == *right_it) {
            ++match_count;
        }
        ++right_it;
    }

    return match_count;
}

std::size_t Code::checkIncorrect(const Code& other) const
{
    // Sequence of digits from this->m_digits that do not match other.m_digits
    // in value and position.
    std::vector<Digit> differing_digits_left{};
    // Sequence of digits from other.m_digits that do not match this->m_digits
    // in value and position.
    std::vector<Digit> differing_digits_right{};

    auto right_it = std::begin(other.m_digits);

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

    std::vector<Digit> non_matching_digits{};

    // Compute the difference between the two digit sequences using set operations.
    std::set_difference(
        std::begin(differing_digits_left), std::end(differing_digits_left),
        std::begin(differing_digits_right), std::end(differing_digits_right),
        std::inserter(non_matching_digits, std::begin(non_matching_digits))
    );

    return differing_digits_left.size() - non_matching_digits.size();

}

std::ostream& operator<<(std::ostream& out, const Code& code)
{
    out << "[ ";
    for (Code::Digit digit : code.m_digits) {
        out << static_cast<int>(digit) << ' ';
    }
    out << ']';

    return out;
}
