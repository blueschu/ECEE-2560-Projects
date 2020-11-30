/**
 * Sudoku board for project 4.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-14
 *
 * References
 * ==========
 *  [1] https://stackoverflow.com/questions/14718055/
 *  [2] https://stackoverflow.com/questions/64794809/#64794991
 *  [3] https://en.cppreference.com/w/cpp/iterator/istream_iterator
 *  [4] https://en.cppreference.com/w/cpp/named_req/Container
 */

#ifndef EECE_2560_PROJECTS_SUDOKU_BOARD_H
#define EECE_2560_PROJECTS_SUDOKU_BOARD_H

#include <algorithm>        // for std::find
#include <array>            // for std::array
#include <cstddef>          // for std::size_t
#include <iostream>         // for I/O stream definitions
#include <limits>           // for std::numeric_limits
#include <memory>           // for std::unique_ptr
#include <type_traits>      // for std::is_integral

#include "eece2560_io.h"
#include "eece2560_iter.h"
#include "matrix.h"

//namespace details {
//template<typename Iter, typename Needle, typename Compare = std::less<>>
//Iter max_bounded(Iter it, Iter end, Needle needle, Needle cap, Compare compare = Compare())
//{
//    Iter current_max;
//    ++it;
//    while (it != end) {
//        if (!compare(*it, cap)) {
//            continue;
//        }
//
//        if (compare(*current_max, *it)) {
//            current_max = it;
//
//            if (!compare(*current_max, needle)) {
//                return current_max;
//            }
//        }
//        ++it;
//    }
//    return current_max;
//}
//}

namespace details {
/**
 * Aggregate storing information about row, column, and block conflicts in
 * a Sudoku board.
 */
template<std::size_t N>
struct BoardConflicts;
} // end namespace details

/**
 * Policy for associating blank cell representations and conflict lookup table
 * indices with types being used as Sudoku cell values.
 *
 * This template can be specialized for user defined types to allow for more
 * interesting classes to be used as Sudoku cell values.
 *
 * This class was created mostly as an excuse for experimenting with policies.
 *
 * @tparam T Entry type for Sudoku cells
 */
template<typename T>
struct SudokuEntryPolicy {
    static_assert(std::is_integral_v<T>);

    /// Value used to represent a blank cell in the Sudoku board.
    const T blank_sentinel{0};

    /**
     * Converts the given entry to a unique array-index in the interval [0, N)
     * where N is any value such that entry_valid(entry, N) returns true.
     */
    constexpr std::size_t index_of(T entry) const { return entry - 1; }

    /**
     * Returns the entry associated with the given index.
     */
    constexpr T reverse_index(std::size_t index) const { return index + 1; }

    /**
     * Returns true if the given entry is a legal value for a board with the
     * given dimension.
     */
    constexpr bool entry_valid(T entry, std::size_t board_dimension) const
    {
        return entry >= 1 && entry <= board_dimension;
    }
};

/**
 * A Sudoku board.
 *
 * @tparam N Characteristic board size, usually 3. The board will be composed
 * of N an N grid of blocks where each block contains an N by N grid of cells.
 * @tparam Entry Type used to represent cell values on the board.
 * @tparam Policy Policy [1] for associating blank cell representation and
 *                conflict lookup table indices with cell entries.
 */
template<std::size_t N, typename Entry = unsigned int, typename Policy = SudokuEntryPolicy<Entry>>
class SudokuBoard {

    /// The number of rows / number of columns on this Sudoku board.
    constexpr static std::size_t k_dim{N * N};

    /// Type used to store the cell entries for this Sudoku board.
    using Board = Matrix<Entry, k_dim>;

    using Conflicts = details::BoardConflicts<k_dim>;

    /// The type used to index cell on this Sudoku board.
    using Coordinate = typename Board::Coordinate;

    using CallCount = unsigned int;

    /**
     * Policy instance for associating Sudoku cell behaviors will arbitrary
     * entry types.
     */
    Policy m_entry_policy;

    /**
     * Free store lookup tables for row, column, and block conflicts.
     *
     * Zero-initialized to all false by default.
     */
    const std::unique_ptr<Conflicts> m_conflicts{new Conflicts{}};

    /// The cell entries for this Sudoku board.
    const std::unique_ptr<Board> m_board_entries{new Board{}};

  public:

    /// Create a Sudoku board with an empty board.
    explicit SudokuBoard(Policy policy = Policy())
        : m_entry_policy(std::move(policy)) {};

    /**
     * Create a Sudoku board with the given initial board values.
     *
     * This constructor DO NOT check whether the given board values are legal.
     * Nonsense will ensue if there are multiple instances of the same vaule in
     * the same row, column, or block.
     */
    explicit SudokuBoard(std::unique_ptr<Board> board, Policy policy = Policy())
        : m_board_entries(std::move(board)), m_entry_policy(std::move(policy))
    {
        auto it = std::begin(*m_board_entries);
        auto end = std::end(*m_board_entries);
        while (it != end) {
            if (!m_entry_policy.check_valid(*it)) {
                *it = m_entry_policy.blank_sentinel;
            } else {
                set_conflict_state(m_board_entries->coordinate_of(it), *it, true);
            }
            ++it;
        }
    }

    /// Returns the number of rows / number of columns on this Sudoku board.
    constexpr static std::size_t dim() { return k_dim; }

    /**
     * Sets the cell at the given index to the given entry and updates the
     * conflict tables accordingly.
     *
     * This function returns true if the board was successfully updated with the
     * new cell value. If the new value conflicts with a value in the same row,
     * column, or block, no change will be made and this function will return
     * false.
     *
     * @param index Board index to be update.
     * @param entry New entry value.
     * @return true if the board was updated with the new cell value.
     */
    bool set_cell(Coordinate coord, Entry entry)
    {
        if (!check_legal_move(coord, entry)) {
            return false;
        }

        auto& cell = (*m_board_entries)[coord];

        if (cell != m_entry_policy.blank_sentinel) {
            // Remove the conflicts associated with the old entry.
            set_conflict_state(coord, cell, false);
        }

        // Write the new entry.
        cell = entry;

        // Add the row, column, and block conflicts for the new entry.
        set_conflict_state(coord, entry, true);

        return true;
    }

    /**
     * Sets the cell at the given coordinate to a blank value and removes the
     * conflicts associated with that entry.
     *
     * If the cell is already blank, this function takes no action.
     *
     * @param coord Board coordinate to be cleared.
     */
    void clear_cell(Coordinate coord)
    {
        auto& cell = (*m_board_entries)[coord];

        if (cell != m_entry_policy.blank_sentinel) {
            // Remove the row, column, and block conflicts associated with
            // the old cell value.
            set_conflict_state(coord, cell, false);
            // Set the entry at the specified coordinate to a blank value.
            cell = m_entry_policy.blank_sentinel;
        }
    }

    /**
     * Fill this Sudoku board with blank entries and removes all conflicts.
     */
    void clear()
    {
        // Fill the Sudoku board with blank entries.
        std::fill(std::begin(*m_board_entries), std::end(*m_board_entries), m_entry_policy.blank_sentinel);
        // Set all of the conflict tables to all false. [2]
        for (auto conflict_table : {
            std::ref(m_conflicts->rows.table),
            std::ref(m_conflicts->cols.table),
            std::ref(m_conflicts->blocks.table)
        }) {
            std::fill(std::begin(conflict_table.get()), std::end(conflict_table.get()), false);
        }
    }

    /**
     * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
     * indicating whether the board was successfully solved, and 1) the number
     * of recursive function calls required to determine the solution.
     *
     * @return Pair of 0) whether the board was solved, 1) the number of
     *         recursive calls made to find the solution.
     */
    std::pair<bool, CallCount> solve_scanning_row()
    {
        const auto first_blank = std::find(
            m_board_entries->cbegin(),
            m_board_entries->cend(),
            m_entry_policy.blank_sentinel
        );
        return solve_after(first_blank, m_board_entries->end());
    }

    /**
     * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
     * indicating whether the board was successfully solved, and 1) the number
     * of recursive function calls required to determine the solution.
     *
     * @return Pair of 0) whether the board was solved, 1) the number of
     *         recursive calls made to find the solution.
     */
    std::pair<bool, CallCount> solve_scanning_column()
    {
        const auto first_blank = std::find(
            m_board_entries->begin_column(),
            m_board_entries->end_column(),
            m_entry_policy.blank_sentinel
        );
        return solve_after(first_blank, m_board_entries->end_column());
    }

    /**
     * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
     * indicating whether the board was successfully solved, and 1) the number
     * of recursive function calls required to determine the solution.
     *
     * @return Pair of 0) whether the board was solved, 1) the number of
     *         recursive calls made to find the solution.
     */
//    std::pair<bool, CallCount> solve_heuristic()
//    {
//        const auto first_blank = std::find(begin(), end(), m_entry_policy.blank_sentinel);
//
//        // Attempts to find the most promising cell occurring after the
//        const auto most_promising = [&](const const_iterator curr_pos) {
//            return std::find(curr_pos, end(), m_entry_policy.blank_sentinel);
//        };
//
//        return solve_after(first_blank, most_promising);

//        const auto first_blank = find_blank({0, 0}, &step_row);
//        if (!first_blank) {
//            return {true, 0};
//        }
//        return solve_after(*first_blank, [&](auto) {
//            auto index = m_conflicts->promising_index(&Conflicts::rows, 5);
//            return find_blank({index, 0}, &step_row);
//        });

//    }

    /**
     * Generates a string representing this Sudoku board.
     *
     * @return Human-readable board representation.
     */
    [[nodiscard]] std::string board_string() const
    {
        using Counter = unsigned int;
        // Make sure the counter technique used below won't overflow.
        static_assert(k_dim * k_dim <= std::numeric_limits<Counter>::max());

        std::ostringstream stream;
        Counter entry_counter{0};

        for (auto entry : *m_board_entries) {
            // Check if the current entry precedes a horizontal-block boundary.
            if (entry_counter % N == 0) {
                stream << "| ";     // Write a vertical divider.
            }

            if (entry_counter != 0) {
                // Check if the current entry precedes a vertical-block boundary
                if (entry_counter % (k_dim * N) == 0) {
                    // Write a horizontal divider with the width of a full row.
                    stream << '\n' << std::string(2 * (k_dim + N) + 1, '-');
                }

                // Check if the current entry is the end of a row.
                if (entry_counter % k_dim == 0) {
                    stream << '\n' << "| ";
                }
            }
            stream << entry << ' ';
            ++entry_counter;
        }
        stream << "|\n";

        return stream.str();
    }

#ifdef EECE2560_PART_A_DEMO
    // Access to the internal conflict implementation for part a demo.
    const Conflicts& debug_conflicts() const { return *m_conflicts; }
#endif

    /// Returns true if this Sudoku board is solved.
    [[nodiscard]] bool fully_solved() const
    {
        // Note - could replace explicit loop with std::find
        for (std::size_t i{0}; i < k_dim * k_dim; ++i) {
            if ((*m_board_entries)[i] == m_entry_policy.blank_sentinel) {
                return false;
            }
        }
        return true;
    };

    /**
     * Returns true if the cell at the given coordinate has no conflicts for
     * the given entry.
     *
     * @param coord Cell whose conflicts are to be checked.
     * @param entry Entry candidate for the cell.
     * @return True if the cell has no conflicts for the entry.
     */
    bool check_legal_move(Coordinate coord, Entry entry)
    {
        const auto[row, col] = coord;
        const auto entry_index = m_entry_policy.index_of(entry);
        return !m_conflicts->check_source(&Conflicts::rows, row, entry_index)
            && !m_conflicts->check_source(&Conflicts::cols, col, entry_index)
            && !m_conflicts->check_source(&Conflicts::blocks, block_index(coord), entry_index);
    }

    /**
     * Sets the `entry` conflict state for the column, row, and block containing
     * the given coordinate.
     *
     * @param coord Cell whose conflict state is being modified.
     * @param entry Value being added or removed as a conflict for the cell's
     *              row, column, and block.
     * @param state Whether the `entry` conflict is being added or removed.
     */
    void set_conflict_state(Coordinate coord, Entry entry, bool state)
    {
        const auto[row, col] = coord;
        const auto block = block_index(coord);
        const auto entry_index = m_entry_policy.index_of(entry);
        m_conflicts->set_source(&Conflicts::rows, row, entry_index, state);
        m_conflicts->set_source(&Conflicts::cols, col, entry_index, state);
        m_conflicts->set_source(&Conflicts::blocks, block, entry_index, state);
    }

    /**
     * Attempts the solve starting at the given position and proceeding left-to-
     * right, top-to-bottom.
     *
     * @tparam Iterator type for the board.
     * @param cell Next cell to attempt to fill with a value..
     * @return Pair of 0) whether the board is solved, 1) the number of recursive
     *         calls required to determine the solution.
     */
    template<typename Iter>
    std::pair<bool, CallCount> solve_after(Iter pos, Iter end)
    {
        unsigned int call_count{1u};

        if (pos == end) {
            return {true, call_count};
        }

        const auto coord = m_board_entries->coordinate_of(pos);

        // Iterate over the k_dim conflict table indices to find entry candidates.
        for (typename Conflicts::Index index{0}; index < k_dim; ++index) {
            // Attempt set the cell at coord with the value associated with index.
            // set_cell returns false if the candidate value has a conflict.
            if (set_cell(coord, m_entry_policy.reverse_index(index))) {

                // Continue solving at the next blank.
                const auto[found_solution, calls] = solve_after(
                    std::find(pos, end, m_entry_policy.blank_sentinel),
                    end
                );
                call_count += calls;

                if (found_solution) {
                    return {true, call_count};
                } else {
                    clear_cell(coord);
                }
            }
        }
        // All possible cell values have a conflict at the current position.
        // Signal to the caller that this branch does not lead to a solution.
        return {false, call_count};
    }

    /**
     * Returns index of the block that contains the given cell.
     *
     * Blocks are numbered left-to-right, top-to-bottom. When N=3, cell (0,0)
     * is in block 0, cell (0, 8) in in block 2, and cell (8,0) is in block 6.
     */
    constexpr static std::size_t block_index(Coordinate coord)
    {
        return N * (coord.first / N) + (coord.second / N);
    }

    // Output stream operator overload.
    friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& sudoku_board)
    {
        eece2560::print_sequence(
            out,
            std::begin(*sudoku_board.m_board_entries),
            std::end(*sudoku_board.m_board_entries),
            ""
        );
        return out;
    }

    // Attempts to read a Sudoku board from the given input stream.
    // Missing and invalid entries are filled with the blank sentinel.
    friend std::istream& operator>>(std::istream& in, SudokuBoard& sudoku_board)
    {
        sudoku_board.clear();

        auto it = std::begin(*sudoku_board.m_board_entries);
        auto end = std::end(*sudoku_board.m_board_entries);

        // Manually iterate over iterator range since we need access to the raw
        // iterator to determine the board coordinate.
        while (it != end) {
            // Each Sudoku entry is represented by a single character.
            char entry_symbol;
            if (!(in >> entry_symbol)) {
                // We can no longer read a character from the input stream.
                // Stop reading the Sudoku board - all remaining entries will be blank.
                break;
            }

            // Place the character into a string stream so that we can use
            // Entry's operator>> overload to convert it into an entry.
            std::stringstream symbol_stream;
            symbol_stream << entry_symbol;

            if (Entry entry_candidate;
                symbol_stream >> entry_candidate
                    && sudoku_board.m_entry_policy.entry_valid(entry_candidate, sudoku_board.k_dim)
                ) {
                // todo decide how to handle invalid sudoku boards. For now we silently omit illegal entries.
                sudoku_board.set_cell(
                    sudoku_board.m_board_entries->coordinate_of(it),
                    entry_candidate
                );
            }
            ++it;
        }

        return in;
    }
};

namespace details {
/**
 * Aggregate storing information about row, column, and block conflicts in
 * a Sudoku board.
 */
template<std::size_t N>
struct BoardConflicts {
    using Table = Matrix<bool, N>;
    using Count = unsigned int;
    using Index = typename Table::size_type;

    struct ConflictSource {
        /*
         * Grid of k_dim rows where each row stores whether the entry associated
         * with a given column has a conflict elsewhere in the row/column/block.
         */
        Table table;

        /// Cache of the number of conflicts present in each row of the conflict table.
        std::array<Count, N> counts;
    };

    using Source = ConflictSource BoardConflicts::*;

    ConflictSource rows;
    ConflictSource cols;
    ConflictSource blocks;

    /**
     *
     * @param source_index Which row/column/block.
     * @param entry_index Which entry within the row/column/block.
     */
    void set_source(const Source source, Index source_index, Index entry_index, bool state)
    {
        // Whether the given entry has a conflict in the row/column/block indexed by source_index.
        bool& conflict_flag = (this->*source).table[{source_index, entry_index}];

        // Update the conflict count for row/column/block.
        if (conflict_flag != state) {
            if (state) {
                (this->*source).counts[source_index] += 1;
            } else {
                (this->*source).counts[source_index] -= 1;
            }
        }
        conflict_flag = state;
    }

    [[nodiscard]] bool check_source(const Source source, Index source_index, Index entry_index) const
    {
        return (this->*source).table[{source_index, entry_index}];
    }

//        Count conflict_count(const Source source, Index source_index) const
//        {
//            return (this->*source).counts[source_index];
//        }

    [[nodiscard]] Index promising_index(const Source source) const
    {
        constexpr static auto not_full = [](Count count) { return count < N; };
        const auto& count_array = (this->*source).counts;
        const auto start = eece2560::make_filter_iter(count_array, not_full);
        const auto end = eece2560::make_filter_iter_end(count_array, not_full);
        const auto pos = std::max_element(start, end);
        return static_cast<Index>(std::distance(start, pos));
    }

};

/// Ensure that Conflicts is an aggregate.
static_assert(std::is_aggregate_v<BoardConflicts<1>>);

} // end namespace details

#endif //EECE_2560_PROJECTS_SUDOKU_BOARD_H
