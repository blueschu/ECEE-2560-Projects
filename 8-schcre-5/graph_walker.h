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
 *  [3] https://en.cppreference.com/w/cpp/utility/functional/not_fn
 *
 */

#ifndef EECE_2560_PROJECTS_GRAPH_WALKER_H
#define EECE_2560_PROJECTS_GRAPH_WALKER_H

#include <algorithm>        // for std::fill, std::reverse
#include <functional>       // for std::not_fn
#include <optional>         // for std::optional
#include <numeric>          // for std::iota
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
        init(graph);
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
        init(graph);

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

        // Lambda comparing graph indices by their shortest path length from the
        // start node. Nodes which currently have no path leading are treated
        // as though their path length was infinite
        const auto compare_paths = [&](GraphIndex lhs, GraphIndex rhs) {
            const auto& lhs_path = shortest_paths[lhs];
            const auto& rhs_path = shortest_paths[rhs];

            // If there exists no path to the lhs node, we consider its path
            // length to be infinite, therefore lhs < rhs is false for all rhs.
            if (!lhs_path) {
                return false;
            }

            // If the lhs node has a path to it, but the rhs node does not, this
            // comparison is treated as (finite) < (infinite) = true.
            if (!rhs_path) {
                return true;
            }

            // Both nodes have paths to them, so we compare their associated
            // path lengths.
            return lhs_path->total_weight < rhs_path->total_weight;
        };

        const auto heap_start = std::begin(unvisited_indices);
        auto heap_end = std::end(unvisited_indices);

        // By default, std::make_heap creates a max-heap. Since we want a min-heap,
        // we invert the ordering imposed by compare_paths.
        std::make_heap(heap_start, heap_end, std::not_fn(compare_paths));

        while (true) {
            // Pop the unvisited node with the shortest path off the heap.
            std::pop_heap(heap_start, heap_end, std::not_fn(compare_paths));
            --heap_end;
            // Access the popped node.
            const GraphIndex current_index = *heap_end;

            // Mark the current node as visited.
            m_visited[current_index] = true;

            if (current_index == goal.index()) {
                if (!shortest_paths[current_index]) {
                    // The current node has no path associated with it. This
                    // can only occur when we have exhausted all nodes that have
                    // paths between them and the start node. The goal node must
                    // be isolated from the starting node.
                    return {{}, {}};
                }

                // The target node has been found. Reconstruct the path.
                std::vector<GraphIndex> path;
                path.push_back(current_index);

                GraphIndex retrace_index = current_index;
                while (retrace_index != start.index()) {
                    retrace_index = shortest_paths[retrace_index]->parent_index;
                    path.push_back(retrace_index);
                }
                std::reverse(std::begin(path), std::end(path));
                return {path, shortest_paths[current_index]->total_weight};
            }

            // Update the shortest paths to the neighbors of the current node.
            for (const auto& [neighbor, edge_weight] : graph[current_index].neighbors()) {
                const GraphIndex nb_index = neighbor.index();
                if (m_visited[nb_index]) {
                    continue;
                }

                const Weight new_weight = shortest_paths[current_index]->total_weight + edge_weight;

                // If the neighbor node has not associated path, or if the current shortest path
                // to it is longer than the newly computed path, update the neighbor
                // node's shortest path.
                if (const auto& nb_path = shortest_paths[nb_index];
                    !nb_path || new_weight < shortest_paths[nb_index]->total_weight) {
                    shortest_paths[nb_index] = {new_weight, current_index};
                }

            }

            // Re-heapify the heap since the shortest paths to the nodes will
            // have changed.
            std::make_heap(heap_start, heap_end, std::not_fn(compare_paths));
        }
    }

  private:

    /**
     * Initializes the state of this graph walker so that it can traverse the
     * given graph.
     *
     * This function should be called prior the the execution of any graph
     * traversal algorithm.
     *
     * @param graph The graph to be traversed.
     */
    void init(const GraphType& graph)
    {
        m_visited.resize(graph.size());
        std::fill(std::begin(m_visited), std::end(m_visited), 0);
    }

    /**
     * Attempts to find a path between start and goal using a depth-first
     * searching algorithm.
     *
     * This algorithm is implemented recursively per the project instructions.
     *
     * @param start The starting node in the graph.
     * @param goal The desired end node to be navigated to.
     * @return Search result containing a path and its total weight, if a path was found.
     */
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
