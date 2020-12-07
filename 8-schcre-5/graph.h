/**
 * Graph class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 * References
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/named_req/Container
 *  [2] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef EECE_2560_PROJECTS_GRAPH_H
#define EECE_2560_PROJECTS_GRAPH_H

#include <algorithm>        // for std::transform
#include <optional>         // for std::optional

#include "matrix.h"

/**
 * A directed graph that stores edges using an adjacency matrix.
 *
 * @tparam Node
 * @tparam Edge
 */
template<typename Node, typename Edge>
class Graph {
  public:
    class NodeHandle;

    // Type aliases for C++ container [1].
    using value_type = NodeHandle;
    using reference = NodeHandle&;
    using const_reference = const NodeHandle&;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    class NodeHandle {
        /// The graph this node is associated with.
        Graph* m_graph;

        /// This node's index in its graph.
        size_type m_index;

        /// This node's contents.
        Node m_node;

        /// Creates a handle for the given node in the given graph, which is
        /// is located at the specified index in the graph.
        NodeHandle(Graph& graph, size_type index, Node node)
            : m_graph(&graph), m_index(index), m_node(std::move(node)) {}

      public:
        /// This node's index in its graph.
        [[nodiscard]] size_type index() const noexcept { return m_index; }

        void connect(const NodeHandle& other, const Edge& edge)
        {
            m_graph->connect_indices(m_index, other.m_index, edge);
        }

        void connect(const NodeHandle& other, Edge&& edge)
        {
            m_graph->connect_indices(m_index, other.m_index, std::forward<Edge>(edge));
        }

        /**
         * Returns a the neighbors of this node. Each neighbor is described by
         * a pair containing 1) a handle for the neighboring node, and 2) a
         * reference to the edge connects the two nodes.
         *
         * @return The neighbors of this node.
         */
        std::vector<std::pair<NodeHandle, const Edge&>> neighbors() const
        {
            const auto row = m_index;
            std::vector<std::pair<NodeHandle, const Edge&>> result;

            for (size_type col{0}; col < m_graph->m_edges.dimensions().second; ++col) {
                if (const auto& edge = m_graph->m_edges[{row, col}]) {
                    result.emplace_back(m_graph->m_nodes[col], *edge);
                }
            }
            return result;
        }

        Node& operator*() { return m_node; }

        const Node& operator*() const { return m_node; }

        friend Graph;
    };

  private:
    std::vector<value_type> m_nodes;

    Matrix<std::optional<Edge>> m_edges;

  public:

    explicit Graph(std::vector<Node> nodes)
        : m_edges(std::vector(nodes.size() * nodes.size(), std::optional<Edge>{}))
    {
        size_type counter{0};
        std::transform(
            std::make_move_iterator(std::begin(nodes)),
            std::make_move_iterator(std::end(nodes)),
            std::back_inserter(m_nodes),
            [&](Node&& n) { return NodeHandle(*this, counter++, std::move(n)); }
        );
        m_edges.reshape({m_nodes.size(), m_nodes.size()});
    }

    void connect_indices(size_type from, size_type to, const Edge& edge)
    {
        m_edges[{from, to}] = edge;
    }

    void connect_indices(size_type from, size_type to, Edge&& edge)
    {
        m_edges[{from, to}] = std::make_optional(std::forward<Edge>(edge));
    }

    /// Returns the number of nodes in this graph.
    [[nodiscard]] std::size_t size() const { return m_nodes.size(); }

    iterator begin() noexcept { return std::begin(m_nodes); }

    iterator end() noexcept { return std::end(m_nodes); }

    const_iterator begin() const noexcept { return std::begin(m_nodes); }

    const_iterator end() const noexcept { return std::end(m_nodes); }
};

#endif //EECE_2560_PROJECTS_GRAPH_H
