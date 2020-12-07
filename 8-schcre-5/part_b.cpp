/**
 * Project 5b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#include <array>                // for std::array
#include <iostream>             // for I/O stream definitions

#include "maze.h"
#include "graph_walker.h"

constexpr auto k_maze_files = std::array{
    "resources/maze1.txt",
    "resources/maze2.txt",
    "resources/maze3.txt"
};

int main()
{
    for (const auto file_name : k_maze_files) {
        std::cout << file_name << ":\n";
        const auto maze = Maze::read_file(file_name);
        auto graph = maze.make_graph();

        // Uncomment to see the generated graph
//        for (const auto& node : graph) {
//            std::cout << '(' << (*node).first << ',' << (*node).second << "):";
//            for (const auto& neighbor : node.neighbors()) {
//                std::cout << ' ' << '(' << (*neighbor).first << ',' << (*neighbor).second << ')';
//            }
//            std::cout << '\n';
//        }

        GraphWalker<Maze::Coordinate, Maze::PathWeight> walker;
        auto result = walker.find_path_dfs(graph, *std::begin(graph), *(std::end(graph) - 1));
        if (result) {
            std::cout << "Path: ";
            for (const auto& node : result.path) {
                std::cout << '(' << node->first << ',' << node->second << "), ";
            }
            std::cout << "\nDistance: " << result.weight;
        } else {
            std::cout << "No path found.";
        }


        std::cout << "\n\n";
    }

}
