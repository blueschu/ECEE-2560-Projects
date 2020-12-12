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
    /// The possible values for the maze tiles. This enum was implemented
    /// partly to avoid using vector<bool>.
    enum class Tile { Path, Blocked };

    /// Type used to represent a grid coordinate for the maze.
    using Coordinate = Matrix<Tile>::Coordinate;

    /// Integral type used for edge weights in maze graphs.
    using PathWeight = int;

    /// Edge weight for all maze paths.
    constexpr static PathWeight k_path_weight{1};   // implicitly inline

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

    /**
     * Returns a pair representing human-readable directions for the given path
     * through this maze.
     *
     * The returned pair consists of 1) a sequence of strings describing each
     * move one must make in the maze, 2) a string containing a 2D ascii
     * rendering of the path through the maze.
     *
     * @param path The path through this maze.
     * @return Human readable directions for the given path.
     */
    [[nodiscard]]
    std::pair<std::vector<std::string>, std::string>
    human_directions(const std::vector<Coordinate>& path) const;

};

std::istream& operator>>(std::istream& in, Maze::Tile& tile);

#endif //EECE_2560_PROJECTS_MAZE_H
