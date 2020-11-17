/**
 * Project 4a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-14
 *
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "sudoku_board.h"

[[maybe_unused]]
constexpr const char k_default_sudoku_file[]{"resources/sudoku1.txt"};

struct SudokuEntry {
    using Value = unsigned int;

    constexpr static char k_blank_symbol{'.'};

    Value value;

    constexpr bool operator==(SudokuEntry rhs) const { return value == rhs.value; }

    constexpr bool operator!=(SudokuEntry rhs) const { return !(rhs == *this); }

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

static_assert(std::is_aggregate_v<SudokuEntry>);

template<>
struct SudokuEntryPolicy<SudokuEntry> {
    const SudokuEntry blank_sentinel{0};

    constexpr std::size_t index_of(SudokuEntry entry) const { return entry.value - 1; }

    constexpr bool entry_valid(SudokuEntry entry, std::size_t board_dimension) const
    {
        return entry.value > 0 && entry.value <= board_dimension;
    }

    constexpr SudokuEntry entry_of(std::size_t index) const { return SudokuEntry{static_cast<SudokuEntry::Value>(index + 1)}; }
};

int main()
{
    SudokuBoard<3, SudokuEntry> board;
    std::istringstream stream(".....2.......7...17..3...9.8..7......2.89.6...13..6....9..5.824.....891..........");
//    std::istringstream stream("3...8.......7....51..............36...2..4....7...........6.13..452...........8..");
    stream >> board;
    std::cout << "Start: " << board << '\n' << std::flush;
    auto solved = board.solve();
    std::cout << "End :  " << board << std::boolalpha << " (" << solved.first << ',' << solved.second << ")\n";

    std::cout << board.board_string();



//    constexpr std::size_t k_title_width{24};
//
//    // Default entry formatting
//    {
//        SudokuBoard<3> board;
//        board.set_cell({0, 1}, 2);
//        std::cout << std::setw(k_title_width) << "Default formatting: " << board << '\n';
//    }
//
//    // Specialized entry formatting.
//    {
//        SudokuBoard<3, SudokuEntry> board;
//        board.set_cell({0, 1}, {2});
//        std::cout << std::setw(k_title_width) << "Specialized formatting: " << board << '\n';
//    }
//
//    // Read from stream.
//    for (auto input_str : {
//        ".....2.......7...17..3...9.8..7......2.89.6...13..6....9..5.824.....891..........",                // example from doc
//        "01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......",                   // too short
//        "01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......321111111111111111"  // too long
//    }) {
//        SudokuBoard<3, SudokuEntry> board;
//        std::istringstream stream(input_str);
//
//        stream >> board;
//        std::cout << std::setw(k_title_width) << "Read from stream: " << board << '\n';
//    }
//
//    // Read from file.
//    {
//        std::cout << std::setw(k_title_width) << "Read from file: ";
//        SudokuBoard<3, SudokuEntry> board;
//        std::ifstream file_in(k_default_sudoku_file);
//
//        std::string line;
//        while (std::getline(file_in, line)) {
//            std::istringstream stream(line);
//            stream >> board;
//            std::cout << board << '\n';
//        }
//
//    }
}
