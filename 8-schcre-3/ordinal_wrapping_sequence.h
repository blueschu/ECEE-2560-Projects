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

namespace details {
/// Compute value % base such that the result is always in [0, base).
template<typename T>
constexpr T positive_mod(T value, T base)
{
    return (value + base) % base;
}
} // end namespace details

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
  public:
    /*
     * Standard aliases for iterator traits [1].
     */
    using value_type = std::vector<T>;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

  private:
    /// Type used to access elements of the underlying matrix.
    using Index = typename Matrix<T>::Index;

    /// The current direction of iteration for producing sequence elements.
    enum { N, NE, E, SE, S, SW, W, NW } m_dir{N};

    /// The matrix being iterated over. This pointer will be null in the end sentinel.
    const Matrix<T>* m_grid_ref;

    /// The position of the current starting elements of the sequence
    Index m_curr_center{};

    /// The position of the current ending element in the sequence.
    Index m_curr_pos{};

    /**
     * The sequence of consecutive elements from the matrix produce by iterating
     * over entries along an ordinal direction.
     *
     * We use the same buffer to store all sequences produced by this iterator.
     * This is safe since the rules for input iterators state that incrementing
     * the iterator is allowed to invalidate any existing copies of the iterator.
     */
    value_type m_sequence{};

  public:
    /// Creates an end iterator.
    OrdinalWrappingSequenceIter() noexcept: m_grid_ref{nullptr} {}

    /// Creates an iterator over the given matrix.
    explicit OrdinalWrappingSequenceIter(const Matrix<T>& grid) : m_grid_ref(&grid)
    {
        const auto[rows, cols] = m_grid_ref->dimensions();
        // Preallocate the storage required to store the longest candidate word.
        m_sequence.reserve(std::max(rows, cols));
        m_sequence.push_back((*m_grid_ref)[m_curr_center]);
    }

    // Dereference operator overload.
    reference operator*() const { return m_sequence; }

    // Arrow operator overload.
    pointer operator->() const { return &m_sequence; }

    // Equality operator overload.
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

    // Pre-increment operator overload.
    OrdinalWrappingSequenceIter& operator++()
    {
        advance();
        if (m_curr_pos == m_curr_center) {
            m_sequence.clear();
            m_sequence.push_back((*m_grid_ref)[m_curr_pos]);
            change_dir();

            if (m_grid_ref) {
                advance();
            } else {
                // Return prematurely before the candidate is updated again.
                return *this;
            }
        }
        m_sequence.push_back((*m_grid_ref)[m_curr_pos]);

        return *this;
    }

    // Post-increment operator overload.
    OrdinalWrappingSequenceIter operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

  private:

    /// Increase the length of this iterators sequence by one in the current direction.
    void advance()
    {
        const auto[rows, cols] = m_grid_ref->dimensions();
        auto offset = compute_offset();

        Index next {
            details::positive_mod(static_cast<int>(m_curr_pos.first) + offset.first, static_cast<int>(cols)),
            details::positive_mod(static_cast<int>(m_curr_pos.second) + offset.second, static_cast<int>(rows))
        };

        m_curr_pos = next;
    }

    /// Rotates the direction of this iterator. Updates the "center" position
    /// if this iterator has completed a full rotation.
    void change_dir()
    {
        switch (m_dir) {
            case N: { m_dir = NE; break;}
            case NE:{ m_dir = E; break; }
            case E: { m_dir = SE; break; }
            case SE:{ m_dir = S; break; }
            case S: { m_dir = SW; break; }
            case SW:{ m_dir = W; break; }
            case W:{ m_dir = NW; break; }
            case NW: {
                m_dir = N;
                advance_center();
            };
        }
    }

    /// Updates the center position of this iterator, proceeding left-to-right,
    /// top-to-bottom.
    void advance_center() {
        const auto[rows, cols] = m_grid_ref->dimensions();
        m_curr_center.first += 1;
        if (m_curr_center.first == cols) {
            m_curr_center.first = 0;
            m_curr_center.second += 1;
        }
        if (m_curr_center.second == rows) {
            m_grid_ref = nullptr;
        }
        m_curr_pos = m_curr_center;
    }

    /// Returns the index offset corresponding to this iterator's current direction.
    [[nodiscard]] std::pair<int, int> compute_offset() const
    {
        switch (m_dir) {
            case N: return {-1, 0};
            case NE: return {-1, 1};
            case E: return {0, 1};
            case SE: return {1, 1};
            case S: return {1, 0};
            case SW: return {1, -1};
            case W: return {0, -1};
            case NW: return {-1, -1};
        }
    }
};

#endif //EECE_2560_PROJECTS_ORDINAL_WRAPPING_SEQUENCE_H
