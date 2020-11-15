/**
 * Project 4a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-14
 *
 */

#include <iostream>

#include "suduko_board.h"

struct SudokuEntry {

    constexpr static char k_blank_symbol{'.'};

    unsigned int value;

    constexpr bool operator==(SudokuEntry rhs) const { return value == rhs.value; }

    constexpr bool operator!=(SudokuEntry rhs) const { return !(rhs == *this); }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
    operator unsigned int() const { return value; }
#pragma clang diagnostic pop

    friend std::istream& operator>>(std::istream& in, SudokuEntry& entry)
    {
        in >> entry.value;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, SudokuEntry entry)
    {
        if (entry.value == 0) {
            out << k_blank_symbol;
        } else {
            out << entry.value;
        }
        return out;
    }
};

template<>
class SudukoEntryPolicy<SudokuEntry> {
  public:
    const SudokuEntry blank_sentinel{0};

    constexpr std::size_t index_of(SudokuEntry entry) const { return entry.value - 1; }

    constexpr bool entry_valid(SudokuEntry entry, std::size_t board_dimension) const
    {
        return entry.value > 0 && entry.value <= board_dimension;
    }
};

int main()
{
    // Default entry formatting
//    SudukoBoard<3> board;
//    board.set_cell({0, 1}, 2);

    // Specialized entry formatting.
    SudukoBoard<3, SudokuEntry> board;
    board.set_cell({0, 1}, {2});
    std::cout << board << '\n';

//    std::stringstream stream(".....2.......7...17..3...9.8..7......2.89.6...13..6....9..5.824.....891..........");
//    std::stringstream stream("01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......");       // too short
    std::stringstream stream("01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......321111111111111111");       // too long

    stream >> board;
    std::cout << board;
}
