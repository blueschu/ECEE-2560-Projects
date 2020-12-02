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

#include <algorithm>        // for std::random_shuffle
#include <array>            // for std::array
#include <cstddef>          // for std::size_t
#include <iostream>         // for I/O stream definitions
#include <limits>           // for std::numeric_limits
#include <memory>           // for std::unique_ptr
#include <numeric>          // for std::iota
#include <optional>         // for std::optional
#include <type_traits>      // for std::is_integral

#include "eece2560_io.h"
#include "matrix.h"

namespace details {
/**
 * Returns an iterator to the greatest element in the given range that is less
 * than cap.
 */
template<typename Iter, typename Needle, typename Compare = std::less<>>
Iter max_bounded(Iter it, Iter end, Needle cap, Compare compare = Compare())
{
    Iter current_max{it};
    ++it;
    while (it != end) {
        if (!compare(*it, cap)) {
            ++it;
            continue;
        }

        if (compare(*current_max, *it)) {
            current_max = it;
        }
        ++it;
    }
    return current_max;
}

template<typename T, typename F, typename P>
std::optional<T> iterate_optional_until(T initial_value, F func = F(), P pred = P())
{
    std::optional<T> current_value = std::optional(std::move(initial_value));
    while (current_value && !pred(*current_value)) {
        current_value = func(*current_value);
    }
    return current_value;
}

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
        const static auto find_next_row = [&](Coordinate coord) {
            return details::iterate_optional_until(coord, step_row, [&](Coordinate c) {
                return ((*m_board_entries)[c] == m_entry_policy.blank_sentinel);
            });
        };

        const auto start = find_next_row({0, 0});
        if (!start) {
            // The board is already solved.
            return {true, 0};
        }
        return solve_after(*start, find_next_row);
    }

    /**
     * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
     * indicating whether the board was successfully solved, and 1) the number
     * of recursive function calls required to determine the solution.
     *
     * @return Pair of 0) whether the board was solved, 1) the number of
     *         recursive calls made to find the solution.
     */
    std::pair<bool, CallCount> solve_scanning_col()
    {
        const static auto find_next_col = [&](Coordinate coord) -> std::optional<Coordinate> {
            return details::iterate_optional_until(coord, step_col, [&](Coordinate c) {
                return ((*m_board_entries)[c] == m_entry_policy.blank_sentinel);
            });
        };

        const auto start = find_next_col({0, 0});
        if (!start) {
            // The board is already solved.
            return {true, 0};
        }
        return solve_after(*start, find_next_col);
    }

    /**
    * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
    * indicating whether the board was successfully solved, and 1) the number
    * of recursive function calls required to determine the solution.
    *
    * @return Pair of 0) whether the board was solved, 1) the number of
    *         recursive calls made to find the solution.
    */
    std::pair<bool, CallCount> solve_scanning_block()
    {
        const static auto find_next_block = [&](Coordinate coord) -> std::optional<Coordinate> {
            return details::iterate_optional_until(coord, step_block, [&](Coordinate c) {
                return ((*m_board_entries)[c] == m_entry_policy.blank_sentinel);
            });
        };

        auto start = find_next_block({0, 0});
        if (!start) {
            // The board is already solved.
            return {true, 0};
        }
        return solve_after(*start, find_next_block);
    }

    /**
     * Attempts to solve this Sudoku board. Returns a pair containing 0) a bool
     * indicating whether the board was successfully solved, and 1) the number
     * of recursive function calls required to determine the solution.
     *
     * @return Pair of 0) whether the board was solved, 1) the number of
     *         recursive calls made to find the solution.
     */
    std::pair<bool, CallCount> solve_heuristic()
    {
        const static auto guess_next = [&](auto) -> std::optional<Coordinate> {
            const auto best_row = m_conflicts->promising_index(&Conflicts::rows);
            const auto best_col = m_conflicts->promising_index(&Conflicts::cols);

            if (!best_row) {
                // The board is filled.
                return std::nullopt;
            }

            if (best_row->second > best_col->second) {
                return details::iterate_optional_until(Coordinate{best_row->first, 0}, step_row, [&](Coordinate c) {
                    return ((*m_board_entries)[c] == m_entry_policy.blank_sentinel);
                });
            } else {
                return details::iterate_optional_until(Coordinate{0, best_col->first}, step_col, [&](Coordinate c) {
                    return ((*m_board_entries)[c] == m_entry_policy.blank_sentinel);
                });
            }
        };
        const auto start = guess_next(Coordinate{});
        if (!start) {
            // The board is already solved.
            return {true, 0};
        }

        return solve_after(*start, guess_next);
    }

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
    [[nodiscard]] bool is_solved() const
    {
        // Note - could replace explicit loop with std::find
        for (std::size_t i{0}; i < k_dim * k_dim; ++i) {
            if ((*m_board_entries)[i] == m_entry_policy.blank_sentinel) {
                return false;
            }
        }
        return true;
    };

  private:

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
     * Attempts to solve this sudoku puzzle starting at the given coordinate.
     *
     * @tparam F Functor that returns the next blank cell to be filled.
     * @param cell Next cell to attempt to fill with a value..
     * @return Pair of 0) whether the board is solved, 1) the number of recursive
     *         calls required to determine the solution.
     */
    template<typename F>
    std::pair<bool, CallCount> solve_after(Coordinate coord, F find_next_blank = F())
    {
        static_assert(std::is_same_v<std::invoke_result_t<F, Coordinate>, std::optional<Coordinate>>);

        CallCount call_count{1u};

        // Generate an array of all entry indices
        auto m_entry_indices = []() {
            std::array<typename Conflicts::Index, k_dim> temp;
            std::iota(std::begin(temp), std::end(temp), 0);
            return temp;
        }();

        // Sort the entry indices based on the number of times each entry appears in the board.
        std::sort(std::begin(m_entry_indices), std::end(m_entry_indices), [&](auto lhs, auto rhs) {
            return m_conflicts->entry_counts[lhs] < m_conflicts->entry_counts[rhs];
        });


        // Iterate over the k_dim conflict table indices to find entry candidates.
        for (const auto index : m_entry_indices) {
            // Attempt set the cell at coord with the value associated with index.
            // set_cell returns false if the candidate value has a conflict.
            if (set_cell(coord, m_entry_policy.reverse_index(index))) {

                auto next = find_next_blank(coord);

                if (!next) {
                    // There are no remaining blank cells - the board has been solved.
                    return {true, call_count};
                }

                // Continue solving at the next blank.
                const auto[found_solution, calls] = solve_after(*next, find_next_blank);
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

    /// Returns the coordinate that follows `coord` when scanning a board left-
    /// to-right, top-to-bottom.
    constexpr static std::optional<Coordinate> step_row(Coordinate coord)
    {
        Coordinate next = {coord.first, coord.second + 1};
        if (next.second >= k_dim) {
            next.first += 1;
            next.second = 0;
            if (next.first == k_dim) {
                return std::nullopt;
            }
        }
        return next;
    }

    /// Returns the coordinate that follows `coord` when scanning a board top-
    /// to-bottom, left-to-right.
    constexpr static std::optional<Coordinate> step_col(Coordinate coord)
    {
        const auto temp = step_row({coord.second, coord.first});

        if (temp) {
            return {{temp->second, temp->first}};
        } else {
            return std::nullopt;
        }
    }

    /// Returns the coordinate that follows `coord` when scanning the board by
    /// blocks, left-to-right, top-to-bottom.
    constexpr static std::optional<Coordinate> step_block(Coordinate coord)
    {
        const auto[row, col] = coord;
        if ((col + 1) % N != 0) {
            // The current coord is NOT on the right edge of a block.
            // Step to the right within the current block.
            return {{row, col + 1}};
        }

        if ((row + 1) % N != 0) {
            // The current coord is NOT on the bottom edge of a block.
            // Move to the next row of the current block.
            return {{row + 1, col + 1 - N}};
        }

        // If control reaches here, the current coord is on the bottom-right
        // corner of a block. We need to determine where the next block begins.

        if ((col + 1) % k_dim == 0) {
            // The current coord in on the right edge of the *board*.
            if (row == k_dim - 1) {
                // The current coord in on the bottom-right corner of the board.
                // There is not next block.
                return std::nullopt;
            } else {
                // The next block is the first block on the next row.
                return {{row + 1, 0}};
            }
        } else {
            // The next block is to the right of this block.
            return {{row + 1 - N, col + 1}};
        }

    }
};

namespace details {
/**
 * Aggregate storing information about row, column, and block conflicts in
 * a Sudoku board.
 *
 * @tparam N The number of rows/columns/blocks in the board.
 */
template<std::size_t N>
struct BoardConflicts {
    /**
     * Matrix used to store whether a conflict exists for each entry value in
     * each row/column/block. Each row in the matrix represents a single row/
     * column/block.
     */
    using Table = Matrix<bool, N>;

    /// Integral type used to store the number of conflicts in a conflict source.
    using Count = unsigned int;

    /// Type used to index the rows of a Table.
    using Index = typename Table::size_type;

    /// Aggregate storing the conflict table togethor with the number of conflicts
    /// contained in each row of the table.
    struct ConflictSource {
        /*
         * Grid of k_dim rows where each row stores whether the entry associated
         * with a given column has a conflict elsewhere in the row/column/block.
         */
        Table table;

        /// Cache of the number of conflicts present in each row of the conflict table.
        std::array<Count, N> counts;
    };

    /// Member pointer to a conflict source. Used to abstract over operations that
    /// may be performed identically on the row, column, or block conflicts.
    using Source = ConflictSource BoardConflicts::*;

    ConflictSource rows;
    ConflictSource cols;
    ConflictSource blocks;

    /// Cache of number of conflicts associated with each entry index.
    std::array<Count, N> entry_counts;

    /// Sets the conflict states of the specified row/column/block for the given entry.
    void set_source(const Source source, Index source_index, Index entry_index, bool state)
    {
        // Whether the given entry has a conflict in the row/column/block indexed by source_index.
        bool& conflict_flag = (this->*source).table[{source_index, entry_index}];

        // Update the conflict count for row/column/block.
        if (conflict_flag != state) {
            if (state) {
                (this->*source).counts[source_index] += 1;
                entry_counts[entry_index] += 1;
            } else {
                (this->*source).counts[source_index] -= 1;
                entry_counts[entry_index] -= 1;
            }
        }
        conflict_flag = state;
    }

    /// Returns the conflict states of the specified row/column/block for the given entry.
    [[nodiscard]] bool check_source(const Source source, Index source_index, Index entry_index) const
    {
        return (this->*source).table[{source_index, entry_index}];
    }

    /**
     * Returns the index of the row/column/block index with the most conflicts
     * that is not fully filled.
     *
     * @param source Row/column/block index.
     * @return Pair containg 0) the row/column/block index, 1) the number of conflicts
     *         in the row/column/block.
     */
    [[nodiscard]] std::optional<std::pair<Index, Count>> promising_index(const Source source) const
    {
        const auto& count_array = (this->*source).counts;

        const auto pos = details::max_bounded(std::cbegin(count_array), std::cend(count_array), N);

        if (pos != std::cend(count_array)) {
            return {{static_cast<Index>(std::distance(std::cbegin(count_array), pos)), *pos}};
        } else {
            // This row/column/block has been filled, so there is not promising index.
            return std::nullopt;
        }
    }

};

/// Ensure that Conflicts is an aggregate.
static_assert(std::is_aggregate_v<BoardConflicts<1>>);

} // end namespace details

#endif //EECE_2560_PROJECTS_SUDOKU_BOARD_H
