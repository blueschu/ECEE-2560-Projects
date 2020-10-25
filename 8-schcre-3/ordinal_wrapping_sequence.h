/**
 * Iterator for producing word search candidates in project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-23
 *
 * References
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/iterator/iterator_traits
 */

#ifndef EECE_2560_PROJECTS_ORDINAL_WRAPPING_SEQUENCE_H
#define EECE_2560_PROJECTS_ORDINAL_WRAPPING_SEQUENCE_H

#include <vector>               // for std::vector

#include "matrix.h"

/**
 * An iterator that produces every consecutive sequence of elements produced
 * by traversing a matrix along each of the eight ordinal directions, starting
 * from each element in the matrix.
 *
 * This iterator is an input iterator. Incrementing this iterator invalidates
 * any existing copies.
 */
template<typename T>
class OrdinalWrappingSequenceIter {
    /*
     * Standard aliases for iterator traits [1].
     */
    using value_type = std::vector<T>;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

  private:

    using Index = typename Matrix<T>::Index;

    enum { N, NE, E, SE, S, SW, W } m_dir{N};

    const Matrix<T>* m_grid_ref;

    Index m_curr_center{};

    Index m_curr_pos{};

    value_type m_candidate{};

  public:
    OrdinalWrappingSequenceIter() noexcept: m_grid_ref{nullptr} {}

    explicit OrdinalWrappingSequenceIter(const Matrix<T>& grid)
        : m_grid_ref(&grid)
    {
        const auto[rows, cols] = m_grid_ref->dimensions();
        // Preallocate the storage required to store the longest candidate word.
        m_candidate.reserve(std::max(rows, cols));
        m_candidate.push_back((*m_grid_ref)[m_curr_center]);
    }

    reference operator*() const { return m_candidate; }

    pointer operator->() const { return &m_candidate; }

    bool operator==(const OrdinalWrappingSequenceIter& rhs) const
    {
        if (!m_grid_ref || !rhs.m_grid_ref) {
            return m_grid_ref == rhs.m_grid_ref;
        }
        return m_curr_center == rhs.m_curr_center && m_curr_pos == rhs.m_curr_pos;
    }

    bool operator!=(const OrdinalWrappingSequenceIter& rhs) const
    {
        return !(rhs == *this);
    }

    OrdinalWrappingSequenceIter& operator++()
    {
        advance();
        if (m_curr_pos == m_curr_center) {
            m_candidate.clear();
            m_candidate.push_back((*m_grid_ref)[m_curr_pos]);
            change_dir();

            if (m_grid_ref) {
                advance();
            } else {
                // Return prematurely before the candidate is updated again.
                return *this;
            }
        }
        m_candidate.push_back((*m_grid_ref)[m_curr_pos]);

        return *this;
    }

  private:

    static std::array<int, 8> make_ordinal_offset(const Matrix<T>& grid)
    {
        const auto row_step = static_cast<int>(grid.dimensions().second);
        return {
            -row_step, -row_step + 1, 1, row_step + 1,
            row_step, row_step - 1, -1, -row_step - 1
        };
    }

    void advance()
    {
        const auto[rows, cols] = m_grid_ref->dimensions();
        auto offset = compute_offset();

        auto next_x = (static_cast<int>(m_curr_pos.first) + offset.first + static_cast<int>(cols)) % static_cast<int>(cols);
        auto next_y = (static_cast<int>(m_curr_pos.second) + offset.second + static_cast<int>(rows)) % static_cast<int>(rows);

        m_curr_pos = {next_x, next_y};
    }

    void change_dir() {
        switch (m_dir) {
            case N: { m_dir = NE; break;}
            case NE:{ m_dir = E; break; }
            case E: { m_dir = SE; break; }
            case SE:{ m_dir = S; break; }
            case S: { m_dir = SW; break; }
            case SW:{ m_dir = W; break; }
            case W: {
                m_grid_ref = nullptr;
            };
        }
    }

    [[nodiscard]]
    std::pair<int, int> compute_offset() const
    {
        switch (m_dir) {
            case N: return {-1, 0};
            case NE: return {-1, 1};
            case E: return {0, 1};
            case SE: return {1, 1};
            case S: return {1, 0};
            case SW: return {1, -1};
            case W: return {0, -1};
        }
    }
};

#endif //EECE_2560_PROJECTS_ORDINAL_WRAPPING_SEQUENCE_H
