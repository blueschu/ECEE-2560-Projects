/**
 * Graph walker class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 * References:
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/algorithm/make_heap
 *  [2] https://en.cppreference.com/w/cpp/algorithm/pop_heap
 *
 */

#ifndef EECE_2560_PROJECTS_GRAPH_WALKER_H
#define EECE_2560_PROJECTS_GRAPH_WALKER_H

#include <algorithm>        // for std::fill, std::reverse
#include <optional>         // for std::optional
#include <numeric>          // for std::iota
#include <tuple>            // for std::tie
#include <set>              // for std::set
#include <vector>           // for std::vector

#include "graph.h"

template<typename Node, typename Weight>
class GraphWalker {

    /// Graph type that this walker will traverse.
    using GraphType = Graph<Node, Weight>;

    /// Node handle typed used by the graph.
    using NodeHandle = typename GraphType::NodeHandle;

    /// Type used to index the nodes of the graph.
    using GraphIndex = typename GraphType::size_type;

    /**
     * Whether each node in the graph being traversed has been visited.
     *
     * Note: be mindful of the fact that std::vector<bool> is not a complete
     * C++ container; some usually container features may not be available.
     */
    std::vector<bool> m_visited;

  public:
    /// Convenience type for expressing the result of a graph search.
    struct PathSearchResult {
        /// Node indices denoting a path on a graph.
        std::vector<GraphIndex> path;
        /// The total weight of the path.
        Weight weight{};

        explicit operator bool() const { return !path.empty(); }
    };

    GraphWalker() = default;

    /**
     * Attempts to find a path between start and goal using a depth-first
     * searching algorithm.
     *
     * This algorithm is implemented recursively per the project instructions.
     *
     * @param graph The graph being traversed.
     * @param start The starting node in the graph.
     * @param goal The desired end node to be navigated to.
     * @return Search result containing a path and its total weight, if a path was found.
     */
    PathSearchResult find_path_dfs(
        const GraphType& graph,
        const NodeHandle& start,
        const NodeHandle& goal)
    {
        prepare_for(graph);
        auto result = find_path_dfs_helper(start, goal);
        std::reverse(std::begin(result.path), std::end(result.path));
        return result;
    }

    /**
     * Attempts to find the shortest path between start and goal using a
     * breadth-first searching algorithm.
     *
     * @param graph The graph being traversed.
     * @param start The starting node in the graph.
     * @param goal The desired end node to be navigated to.
     * @return Search result containing a path and its total weight, if a path was found.
     */
    PathSearchResult find_path_dijkstra(
        const GraphType& graph,
        const NodeHandle& start,
        const NodeHandle& goal)
    {
        prepare_for(graph);

        struct ShortestPath {
            Weight total_weight;
            GraphIndex parent_index;
        };

        // Record of the shortest path to each node with the corresponding index
        // in the graph, if a path has been found.
        std::vector<std::optional<ShortestPath>> shortest_paths(graph.size(), std::nullopt);
        shortest_paths[start.index()] = {0, start.index()};

        // Heap of node indices ordered based on path weight.
        std::vector<GraphIndex> unvisited_indices(graph.size());
        std::iota(std::begin(unvisited_indices), std::end(unvisited_indices), 0);

        const auto compare_paths = [&](GraphIndex lhs, GraphIndex rhs) {
            const auto &lhs_path = shortest_paths[lhs];
            const auto &rhs_path = shortest_paths[rhs];

            // If there exists no path to the lhs node, we want it to compare
            // less than other nodes so that it is sent to the bottom of the heap.
            if (!lhs_path) {
                return true;
            }

            // If the lhs node has a path to it, but the rhs node does not, we
            // want the lhs node to compare greater than the rhs node so that
            // it is brought higher in the heap.
            if (!rhs_path) {
                return false;
            }

            // Both nodes have paths to them. We want the node with the greater
            // weight to compare "less than" the node with the lesser weight so
            // that it is brought upwards in the max heap.
            return shortest_paths[lhs]->total_weight > shortest_paths[rhs]->total_weight;
        };

        auto heap_start = std::begin(unvisited_indices);
        auto heap_end = std::end(unvisited_indices);

        std::make_heap(heap_start, heap_end, compare_paths);

        while (true) {
            std::pop_heap(heap_start, heap_end);
            --heap_end;
            const GraphIndex top_index = *heap_end;
            m_visited[top_index] = true;

            if (top_index == goal.index()) {
                if (!shortest_paths[top_index]) {
                    // There are no more nodes in the heap with paths leading
                    // to them. The goal node must be isolated from the starting
                    // node.
                    return {{}, {}};
                }

                // The target node has been found. Reconstruct the path.
                std::vector<GraphIndex> path;
                path.push_back(top_index);

                auto retrace_index = top_index;
                while (retrace_index != start.index()) {
                    retrace_index = shortest_paths[retrace_index]->parent_index;
                    path.push_back(retrace_index);
                }
                std::reverse(std::begin(path), std::end(path));
                return {path, shortest_paths[top_index]->total_weight};
            }

            for (const auto&[neighbor, edge_weight] : graph[top_index].neighbors()) {
                const auto nb_index = neighbor.index();
                if (m_visited[nb_index]) {
                    continue;
                }

                auto new_weight = shortest_paths[top_index]->total_weight + edge_weight;

                if (auto& nb_path = shortest_paths[nb_index];
                    !nb_path || new_weight < shortest_paths[nb_index]->total_weight) {
                    shortest_paths[nb_index] = {new_weight, top_index};
                }

            }

            // Re-heapify the heap.
            std::make_heap(heap_start, heap_end, compare_paths);
        }

        // The goal index will be yielded from the heap above even if there
        // is no path to it from the starting node. Therefore a branch with
        // a return statement is guaranteed to be reached.
        __builtin_unreachable();
    }

  private:

    void prepare_for(const GraphType& graph)
    {
        m_visited.resize(graph.size());
        std::fill(std::begin(m_visited), std::end(m_visited), 0);
    }

    PathSearchResult find_path_dfs_helper(
        const NodeHandle& current,
        const NodeHandle& goal
    )
    {
        m_visited[current.index()] = true;

        if (current.index() == goal.index()) {
            return {{goal.index()}, Weight{}};
        }

        for (const auto&[neighbor, weight] : current.neighbors()) {
            if (m_visited[neighbor.index()]) {
                continue;
            }

            if (auto result = find_path_dfs_helper(neighbor, goal)) {
                result.path.push_back(current.index());
                result.weight += weight;
                return result;
            }

        }

        m_visited[current.index()] = false;
        return {{}, {}};
    }

};

#endif //EECE_2560_PROJECTS_GRAPH_WALKER_H
