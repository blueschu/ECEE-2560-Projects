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
 */

#ifndef EECE_2560_PROJECTS_SUDUKO_BOARD_H
#define EECE_2560_PROJECTS_SUDUKO_BOARD_H

#include <array>            // for std::array
#include <cstddef>          // for std::size_t
#include <iostream>         // for I/O stream definitions
#include <memory>           // for std::unique_ptr

#include "eece2560_io.h"
#include "matrix.h"

/**
 * Policy for associating blank cell representations and conflict lookup table
 * indices with types being used to represent Sudoku cell values.
 *
 * This template can be specialize for other used defined types to allow most
 * interesting classes to be used as Sudoku cell values.
 *
 * This class was created mostly as an excuse for experimenting with policies.
 *
 * @tparam T Entry type for Sudoku cells
 */
template<typename T>
struct SudukoEntryPolicy {
    static_assert(std::is_integral_v<T>);

    /// Value used to represent a blank cell in the Sudoku board.
    const T blank_sentinel{0};

    /**
     * Converts the given entry to a unique array-index in the interval [0, N)
     * where N is any value such that entry_valid(entry, N) returns true.
     */
    constexpr std::size_t index_of(T entry) const { return entry - 1; }

    /**
     * Returns true if the given entry is a legal value for a board with the
     * given dimension.
     */
    constexpr bool entry_valid(T entry, std::size_t board_dimension) const
    {
        return entry > 0 && entry <= board_dimension;
    }
};

/**
 * A Sudoku board.
 *
 * @tparam N Characteristic board size, usually 3. The board will be composted
 * of N an N grid of blocks where each block contains an N by N grid of cells.
 * @tparam Entry Type used to represent cell values on the board.
 * @tparam Policy Policy [1] for associating blank cell representation and
 *                conflict lookup table indices with cell entries.
 */
template<std::size_t N, typename Entry = unsigned int, typename Policy = SudukoEntryPolicy<Entry>>
class SudukoBoard {

    /// The number of rows / number of columns on this Sudoku board.
    constexpr static std::size_t k_dim{N * N};

    /// Type used to store the cell entries for this Sudoku board.
    using Board = Matrix<Entry, k_dim>;

    /// The type used to index cell on this Sudoku board.
    using Coordinate = typename Board::Coordinate;

    /**
     * Aggregate storing information about row, column, and block conflicts in
     * a Sudoku board.
     */
    struct Conflicts {
        /**
         * Grid of k_dim rows where each row stores whether the entry associated
         * with a given column has a conflict elsewhere in the row/column/block.
         */
        using ConflictTable = Matrix<bool, k_dim>;

        ConflictTable rows;
        ConflictTable cols;
        ConflictTable blocks;

        [[nodiscard]] bool check_row(std::size_t row_index, std::size_t entry_index) const
        {
            rows[{row_index, entry_index}];
        }

        [[nodiscard]] bool check_col(std::size_t col_index, std::size_t entry_index) const
        {
            rows[{col_index, entry_index}];
        }

        [[nodiscard]] bool check_block(std::size_t block_index, std::size_t entry_index) const
        {
            rows[{block_index, entry_index}];
        }
    };

    /// Ensure that Conflicts is an aggregate.
    static_assert(std::is_aggregate_v<Conflicts>);

    /**
     * Policy instance for associating Sudoku cell behaviors will arbitrary
     * cell entry types.
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

    /// Create a Suduko board with an empty board.
    explicit SudukoBoard(Policy policy = Policy())
        : m_entry_policy(std::move(policy)) {};

    /**
     * Create a Suduko board with the given initial board values.
     *
     * This constructor DO NOT check whether the given board values are legal.
     * Nonsense will ensue if there are multiple instances of the same vaule in
     * the same row, column, or block.
     */
    explicit SudukoBoard(std::unique_ptr<Board> board, Policy policy = Policy())
        : m_board_entries(std::move(board)), m_entry_policy(std::move(policy))
    {
        for (auto& entry : *m_board_entries) {
            if (!m_entry_policy.check_valid(entry)) {
                entry = m_entry_policy.blank_sentinel;
            }
        }
    }

    /**
     * Sets the cell at the given index to the given entry and updates the
     * conflict tables accordingly.
     *
     * @param index Board index to be update.
     * @param entry New entry value.
     */
    void set_cell(Coordinate coord, Entry entry)
    {
        if (auto old_entry = (*m_board_entries)[coord]; old_entry != m_entry_policy.blank_sentinel) {
            // Remove the conflicts associated with the old entry.
            set_conflict_state(coord, old_entry, false);
        }

        // Write the new entry.
        (*m_board_entries)[coord] = entry;

        // Add the row, column, and block conflicts for the new entry.
        set_conflict_state(coord, entry, true);
    }

    /**
     * Sets the cell at the given coordinate to a blank value and removes the
     * conflicts associated with that entry.
     *
     * @param coord Board coordinate to be cleared.
     */
    void clear_cell(Coordinate coord)
    {
        // Set the specified coordinate to a blank value.
        const auto old_entry = (*m_board_entries)[coord];
        (*m_board_entries)[coord] = m_entry_policy.blank_sentinel;

        // Remove the row, column, and block conflicts associated with
        // the old cell value.
        set_conflict_state(coord, old_entry, false);
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
            std::ref(m_conflicts->rows),
            std::ref(m_conflicts->cols),
            std::ref(m_conflicts->blocks)
        }) {
            std::fill(std::begin(conflict_table.get()), std::end(conflict_table.get()), false);
        }
    }

  private:
    /**
     * Sets the `entry` conflict state for the column, row, and block containing
     * the given coordinate.
     *
     * @param coord Cell whose conflict state is being modified.
     * @param entry Value being added or removed as a conflict for the cell's
     *              row, column, and block.
     * @param state Whether the `entry` conflict is being added or removed.
     */
    void set_conflict_state(Coordinate coord, Entry entry, bool state) {
        const auto[row, col] = coord;
        m_conflicts->rows[{row, m_entry_policy.index_of(entry)}] = state;
        m_conflicts->cols[{col, m_entry_policy.index_of(entry)}] = state;
        m_conflicts->blocks[{block_index(coord), m_entry_policy.index_of(entry)}] = state;
    }


    /**
     * Returns index of the block that contains the given cell.
     *
     * Blocks are numbered left-to-right, top-to-bottom. When N=3, cell (0,0)
     * is in block 0, cell (0, 8) in in block 2, and cell (8,0) is in block 6.
     */
    static std::size_t block_index(Coordinate coord)
    {
        return N * (coord.first % N) + (coord.second % N);
    }

    friend std::ostream& operator<<(std::ostream& out, const SudukoBoard& suduko_board)
    {
        eece2560::print_sequence(
            out,
            std::begin(*suduko_board.m_board_entries),
            std::end(*suduko_board.m_board_entries),
            ""
        );
        return out;
    }

    // Attempts to read a Sudoku board from the given input stream.
    // Missing and invalid entries are filled with the blank sentinel.
    friend std::istream& operator>>(std::istream& in, SudukoBoard& suduko_board)
    {
        suduko_board.clear();

        for (Entry& entry : *suduko_board.m_board_entries) {
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
                    && suduko_board.m_entry_policy.entry_valid(entry_candidate, suduko_board.k_dim)
                ) {
                entry = entry_candidate;
            }
        }

        return in;
    }
};

#endif //EECE_2560_PROJECTS_SUDUKO_BOARD_H
