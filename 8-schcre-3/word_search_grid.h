/**
 * Grid declarations for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-25
 *
 */

#ifndef EECE_2560_PROJECTS_WORD_SEARCH_GRID_H
#define EECE_2560_PROJECTS_WORD_SEARCH_GRID_H

#include "matrix.h"
#include "ordinal_wrapping_sequence.h"

/**
 * A two-dimensional grid of letters comprising a word search puzzle.
 */
class WordSearchGrid {
    /// The character type used to represent letters in this word search.
    using Entry = char;

    /// This word search's grid of letters.
    Matrix<Entry> m_entries;

  public:
    /// Creates a word search with the given entries.
    explicit WordSearchGrid(Matrix<Entry> entries)
        : m_entries(std::move(entries)) {};

    /// Returns the dimensions of this word search.
    [[nodiscard]] Matrix<Entry>::Coordinate dimensions() const {
        return m_entries.dimensions();
    }

    /**
     * Reads a word search puzzle from the specified file
     *
     * The file should the spaced delimited dimensions of the puzzle on the first
     * line, followed by the sequence of entries.
     */
    static WordSearchGrid read_file(const char* file_name);

     /**
      * Returns an ordinal wrapping sequence iterator starting at the top-left
      * entry of this word search.
      */
    OrdinalWrappingSequenceIter<char> begin() const {
        return OrdinalWrappingSequenceIter<Entry>(m_entries);
    }

    /**
      * Returns an end sentinel ordinal wrapping sequence iterator.
      */
    OrdinalWrappingSequenceIter<char> end() const {
        return OrdinalWrappingSequenceIter<Entry>();
    }

};

#endif //EECE_2560_PROJECTS_WORD_SEARCH_GRID_H
