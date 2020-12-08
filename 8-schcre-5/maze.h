/**
 * Maze class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#ifndef EECE_2560_PROJECTS_MAZE_H
#define EECE_2560_PROJECTS_MAZE_H

#include <iosfwd>

#include "matrix.h"
#include "graph.h"

/**
 * A two-dimensional maze consisting of walls and paths.
 */
class Maze {

  public:
    /// The possible values for the maze tiles.
    enum class Tile { Path, Blocked };

    /// Type used to represent a grid coordinate for the maze.
    using Coordinate = Matrix<Tile>::Coordinate;

    /// Integral type used for edge weights in maze graphs.
    using PathWeight = int;

    /// Edge weight for all maze paths.
    constexpr static PathWeight k_path_weight{1};

  private:
    /// The tiles in this maze.
    Matrix<Tile> m_tiles;

  public:
    /// Create a maze with the given tiles.
    explicit Maze(Matrix<Tile> tiles) : m_tiles(std::move(tiles)) {}

    /// Reads a maze from the given file.
    static Maze read_file(const char* file_name);

    /// Generate a graph representing the legal moves within this maze.
    [[nodiscard]] Graph<Coordinate, PathWeight> make_graph() const;

    /// Returns all of the valid moves from the given position in the maze.
    [[nodiscard]] std::vector<Coordinate> paths_from(Coordinate pos) const;

    /// Returns a string representing this maze with the given path overlaid.
    [[nodiscard]] std::string directions_string(const std::vector<Coordinate>& path) const;

};

std::istream& operator>>(std::istream& in, Maze::Tile& tile);

#endif //EECE_2560_PROJECTS_MAZE_H
