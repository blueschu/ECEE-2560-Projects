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
#include <iomanip>          // for I/O stream manipulators
#include <sstream>          // for std::istringstream

// When defined, the SudokuBoard class will include additional debugging
// functionality which is required for part A of this project.
#define EECE2560_PART_A_DEMO
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

using Board = SudokuBoard<3, SudokuEntry>;

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

void print_conflicts(const Board& board)
{
    constexpr static std::size_t k_label_width{18};
    const static auto legend = [&]() {
        std::vector<std::size_t> legend_values(Board::dim() * Board::dim());
        std::generate(std::begin(legend_values), std::end(legend_values), []() {
            static std::size_t counter{Board::dim() - 1};
            counter = (counter + 1) % Board::dim();
            return counter + 1;
        });
        return legend_values;
    }();

    const auto conflicts = board.debug_conflicts();

    std::cout << std::string(k_label_width + 1, ' ');
    eece2560::print_sequence(std::cout, std::begin(legend), std::end(legend), "", "", "\n");

    std::cout << std::setw(k_label_width) << "Row conflicts: ";
    eece2560::print_sequence(std::cout, std::begin(conflicts.rows), std::end(conflicts.rows), "", "[", "]\n");
    std::cout << std::setw(k_label_width) << "Column conflicts: ";
    eece2560::print_sequence(std::cout, std::begin(conflicts.cols), std::end(conflicts.cols), "", "[", "]\n");
    std::cout << std::setw(k_label_width) << "Block conflicts: ";
    eece2560::print_sequence(std::cout, std::begin(conflicts.blocks), std::end(conflicts.blocks), "", "[", "]\n");
    std::cout << '\n';
}

int main()
{
    unsigned int board_counter{0};
    Board board;

    std::ifstream file_in(k_default_sudoku_file);

    std::string line;
    while (std::getline(file_in, line)) {
        ++board_counter;
        std::istringstream stream(line);
        stream >> board;

        std::cout << "======== Board " << board_counter << " ========\n";
        std::cout << board.board_string();

        print_conflicts(board);
    }
}
