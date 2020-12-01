/**
 * Project 4b main file.
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

#include "sudoku_board.h"

namespace {
/// Relative path to sudoku puzzle file.
constexpr const char k_default_sudoku_file[]{"resources/sudoku.txt"};

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
        in >> std::hex >> entry.value;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, SudokuEntry entry)
    {
        if (entry.value == 0) {
            out << k_blank_symbol;
        } else if (entry.value < 16) {
            out << entry.value;
        } else {
            out << static_cast<char>('a' - 10 + entry.value);
        }
        return out;
    }
};

/// Confirm that SudokuEntry is an aggregate.
static_assert(std::is_aggregate_v<SudokuEntry>);

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp" // Suppress std::srand warnings.

int main()
{
    std::vector<unsigned long> board_call_counts;
    SudokuBoard<3, SudokuEntry> board;

    // We use a weak random number source for simplicity.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::ifstream file_in(k_default_sudoku_file);

    std::string line;
    while (std::getline(file_in, line)) {
        std::istringstream stream(line);
        stream >> board;

        std::cout << "======= Board " << std::setw(3) << board_call_counts.size() << " =======\n";
        std::cout << board.board_string();
        std::cout << "======= Solution ========\n";

        const auto[solved, call_count] = board.solve_heuristic();
        if (solved) {
            std::cout << board.board_string();
        } else {
            std::cout << "No solution exists.\n";
        }
        std::cout << "Total calls made: " << call_count << "\n\n";
        board_call_counts.push_back(call_count);
    }

    std::sort(std::begin(board_call_counts), std::end(board_call_counts));
    const auto board_count = board_call_counts.size();

    const auto median = board_count % 2 == 0
        ? (board_call_counts[board_count / 2 - 1] + board_call_counts[board_count / 2]) / 2
        : board_call_counts[board_count / 2];

    const auto average = static_cast<double>(std::accumulate(
        std::cbegin(board_call_counts),
        std::cend(board_call_counts),
        0ul)) / static_cast<double>(board_count);

    std::cout << std::fixed << std::setprecision(0)
              << "Min. call made:    " << std::setw(8) << board_call_counts[0] << '\n'
              << "Max. call made:    " << std::setw(8) << board_call_counts[board_count - 1] << '\n'
              << "Median calls made: " << std::setw(8) << median << '\n'
              << "Avg. calls made:   " << std::setw(8) << average << '\n';
}

#pragma clang diagnostic pop
