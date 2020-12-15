/**
 * 2D grid  for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 * References
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/iterator/iterator_traits
 *  [2] https://en.cppreference.com/w/cpp/named_req/Container
 *  [3] https://stackoverflow.com/questions/856542/
 */

#ifndef EECE_2560_PROJECTS_MATRIX_H
#define EECE_2560_PROJECTS_MATRIX_H

#include <stdexcept>            // for std::out_of_range
#include <sstream>              // for std::ostring_stream
#include <utility>              // for std::pair
#include <vector>               // for std::vector

/// Exception raised upon accessing a non-existant matrix entry.
struct MatrixIndexError : std::out_of_range {
    // Use parent class constructor.
    using std::out_of_range::out_of_range;
};

/// Exception raised upon attempting to reshape a matrix to an incompatible shape.
struct MatrixResizeError : std::runtime_error {
    // Use parent class constructor.
    using std::runtime_error::runtime_error;
};

/**
 * A two-dimensional, dynamically sized matrix of elements.
 * Not intended for linear algebra.
 *
 * @tparam T Type of elements to be stored.
 */
template<typename T>
class Matrix {

  public:
    /// Container type used to storage matrix elements.
    using Storage = std::vector<T>;

    // Type aliases for C++ container [2].
    using value_type = typename Storage::value_type;
    using reference = typename Storage::reference;
    using const_reference = typename Storage::const_reference;
    using iterator = typename Storage::iterator;
    using const_iterator = typename Storage::const_iterator;
    using difference_type = typename Storage::difference_type;
    using size_type = typename Storage::size_type;

    /// Type used to access matrix elements using a coordinate pair.
    using Coordinate = std::pair<size_type, size_type>;

  private:
    /// Consecutive storage of matrix elements.
    Storage m_entries;

    /// The number of rows in this matrix.
    size_type m_rows;

    /// The number of columns in this matrix.
    size_type m_cols;

  public:

    /// Creates a 1 by 1 matrix with the given scalar value.
    explicit Matrix(T scalar = T()) : m_entries{scalar}, m_rows{1}, m_cols{1} {};

    /// Creates a 1 by N matrix with the given elements.
    explicit Matrix(Storage entries)
        : m_entries(std::move(entries)), m_rows{1}, m_cols{m_entries.size()} {}

    /// Returns the dimensions of this matrix.
    [[nodiscard]] Coordinate dimensions() const noexcept { return {m_rows, m_cols}; }

    /**
     * Attempts to reshape this matrix as a M by N matrix, where [M, N] = new_dim.
     *
     * @param new_dim The new dimensions for this matrix.
     * @throws MatrixResizeError if the entries in this matrix cannot be represented
     *                           as an M by N grid.
     */
    void reshape(Coordinate new_dim)
    {
        const auto[rows, cols] = new_dim;
        if (rows * cols == m_rows * m_cols) {
            m_rows = rows;
            m_cols = cols;
        } else {
            std::ostringstream err_message;
            err_message << "cannot reshape " << m_rows << " by " << m_rows
                        << " matrix to a " << rows << " by " << cols << " matrix";
            throw MatrixResizeError(err_message.str());
        }
    }

    // Returns the entry at the Nth position, counting left-to-right,
    // top-to-bottom, where N=index.
    reference operator[](size_type index)
    {
        // Safely delegate to const implementation [3].
        return const_cast<reference>(
            static_cast<const Matrix*>(this)->operator[](index)
        );
    }

    // Returns the entry at the position (i,j), where both i and j
    // are 0-based indices.
    reference operator[](Coordinate coord)
    {
        // Safely delegate to const implementation [3].
        return const_cast<reference>(
            static_cast<const Matrix*>(this)->operator[](coord)
        );
    }

    // Returns the entry at the Nth position, counting left-to-right,
    // top-to-bottom.
    const_reference operator[](size_type index) const
    {
        if (index >= m_rows * m_cols) {
            throw MatrixIndexError("invalid matrix index");
        }
        return m_entries[index];
    }

    // Returns the entry at the position (i,j), where both i and j
    // are 0-based indices.
    const_reference operator[](Coordinate coord) const
    {
        const auto[row, col] = coord;
        // size_type is an unsigned integer [2], so we only need to check the upper bounds.
        if (row >= m_rows || col >= m_cols) {
            throw MatrixIndexError("invalid matrix index");
        }
        return m_entries[row * m_cols + col];
    }

    /// Returns an iterator to the first (top left) entry of this matrix.
    [[nodiscard]] iterator begin() noexcept { return std::begin(m_entries); }

    /// Returns an iterator to the first (top left) entry of this matrix.
    [[nodiscard]] const_iterator begin() const noexcept { return std::begin(m_entries); }

    /// Returns an iterator to the last (bottom right) entry of this matrix.
    [[nodiscard]] iterator end() noexcept { return std::end(m_entries); }

    /// Returns an iterator to the last (bottom right) entry of this matrix.
    [[nodiscard]] const_iterator end() const noexcept { return std::end(m_entries); }
};

#endif //EECE_2560_PROJECTS_MATRIX_H
