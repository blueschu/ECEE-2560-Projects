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

class Grid {
    using Entry = char;

    Matrix<Entry> m_entries;

  public:
    explicit Grid(Matrix<Entry> entries)
        : m_entries(std::move(entries)) {};

    [[nodiscard]]
    Matrix<Entry>::Index dimensions() const { return m_entries.dimensions(); }

    static Grid read_file(const char* file_name);

};

#endif //EECE_2560_PROJECTS_WORD_SEARCH_GRID_H
