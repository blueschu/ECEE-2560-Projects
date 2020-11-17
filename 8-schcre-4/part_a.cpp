/**
 * Project 4a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-14
 *
 */

#include <fstream>          // for file I/O stream definitions
#include <iostream>         // for I/O stream definition
#include <sstream>          // for std::istringstream

#include "sudoku_board.h"

namespace {
constexpr const char k_default_sudoku_file[]{"resources/sudoku_all.txt"};

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
        } else if (entry.value < 16) {
            out << std::hex << entry.value;
        } else {
            out << static_cast<char>('a' - 10 + entry.value);
        }
        return out;
    }
};

static_assert(std::is_aggregate_v<SudokuEntry>);

} // end namespace

template<>
struct SudokuEntryPolicy<SudokuEntry> {
    const SudokuEntry blank_sentinel{0};

    constexpr std::size_t index_of(SudokuEntry entry) const { return entry.value - 1; }

    constexpr bool entry_valid(SudokuEntry entry, std::size_t board_dimension) const
    {
        return entry.value > 0 && entry.value <= board_dimension;
    }

    constexpr SudokuEntry entry_of(std::size_t index) const
    {
        return SudokuEntry{
            static_cast<SudokuEntry::Value>(index + 1)
        };
    }
};

int main()
{
    unsigned int board_counter{0};
    SudokuBoard<3, SudokuEntry> board;

    std::ifstream file_in(k_default_sudoku_file);

    std::string line;
    while (std::getline(file_in, line)) {
        ++board_counter;
        std::istringstream stream(line);
        stream >> board;

        std::cout << "======== Board " << board_counter << " ========\n";
        std::cout << board.board_string();
        std::cout << "======== Solution ========\n";

        // todo print conflicts

        const auto[solved, call_coutner] = board.solve();
        if (solved) {
            std::cout << board.board_string();
        } else {
            std::cout << "No solution exists.\n";
        }
        std::cout << "Total calls made: " << call_coutner << "\n\n";

    }
}
