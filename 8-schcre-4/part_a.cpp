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
/// Relative path to sudoku puzzle file.
constexpr const char k_default_sudoku_file[]{"resources/sudoku_all.txt"};

/**
 * Simple single-member aggregate class providing custom formatting for sudoku
 * cell entries.
 */
struct SudokuEntry {
    /// Type used to represent an entry's value.
    using Value = unsigned int;

    /// Symbol used to indicate the a sudoku board cell is blank.
    constexpr static char k_blank_symbol{'.'};

    /// This entry's values.
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

/// Confirm that SudokuEntry is an aggregate.
static_assert(std::is_aggregate_v<SudokuEntry>);

// SudokuBoard specialization for part a demo.
using Board = SudokuBoard<3, SudokuEntry>;
} // end namespace

// SudokuEntryPolicy specialization for SudokuEntry. This must be placed in the
// global namespace.
template<>
struct SudokuEntryPolicy<SudokuEntry> {
    const SudokuEntry blank_sentinel{0};

    constexpr std::size_t index_of(SudokuEntry entry) const { return entry.value - 1; }

    constexpr bool entry_valid(SudokuEntry entry, std::size_t board_dimension) const
    {
        return entry.value > 0 && entry.value <= board_dimension;
    }

    constexpr SudokuEntry reverse_index(std::size_t index) const
    {
        return SudokuEntry{
            static_cast<SudokuEntry::Value>(index + 1)
        };
    }
};

namespace {
/**
 * Prints the internal row, column, and block conflicts stored by the given
 * sudoku board.
 *
 * @param board Sudoku board whose conflicts are to be printed.
 */
void print_conflicts(const Board& board)
{
    using namespace std::string_view_literals;
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

    std::cout << std::string(k_label_width + 1, ' ');
    eece2560::print_sequence(std::cout, std::begin(legend), std::end(legend), ""sv, ""sv, "\n"sv);

    const auto& conflicts = board.debug_conflicts();
    for (const auto[label, conflicts] : {
        std::make_pair("Row conflicts: "sv, std::cref(conflicts.rows)),
        std::make_pair("Column conflicts: "sv, std::cref(conflicts.cols)),
        std::make_pair("Block conflicts: "sv, std::cref(conflicts.blocks)),
    }) {
        // Make sure we're not accidentally copying the large aggregate.
        static_assert(std::is_same_v<const Matrix<bool, Board::dim()>&, decltype(conflicts)>);

        std::cout << std::setw(k_label_width) << label;
        eece2560::print_sequence(
            std::cout,
            std::begin(conflicts),
            std::end(conflicts),
            ""sv,
            "["sv,
            "]\n"sv
        );
    }
}
} // end namespace

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
        std::cout << '\n';
    }
}
