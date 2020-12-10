#include <vector>
#include <unordered_set>
#include <cassert>
#include <deque>

// Node class (templated underlying type). Each node contains an adjacency list to builds the Graph
template <typename T>
struct Node
{
    Node(const T &value)
        : data(value)
    {
    }

    void setAdjacencyList(std::vector<Node*> &adj)
    {
        adjacencyList = adj;
    }

    std::vector<T> getAdjacencyList() const
    {
        std::vector<T> values;
        for (const Node *node : adjacencyList)
        {
            values.push_back(node->data);
        }

        return values;
    }

    T data{};
    std::vector<Node*> adjacencyList;
};

// The Graph class contains a sequence of nodes with adjacency lists and provides
// operations for traversal and search
template <typename T>
class Graph
{
public:
    Graph(const std::vector<Node<T>> &nodes)
        : nodes(nodes)
    {
    }

    std::vector<Node<T>> getNodes()
    {
        return nodes;
    }

    // Return elements using depth first searching and recursion
    std::vector<T> depthFirstTraversal() const
    {
        if (nodes.empty())
            return {};
        std::vector<T> values;
        std::unordered_set<T> visited;
        depthFirstImpl(nodes[0], values, visited);
        return values;
    }

    // Return elements breadth first using a double ended queue to maintain state
    std::vector<T> breadthFirstTraversal() const
    {
        if (nodes.empty())
            return {};

        std::vector<T> result;
        std::unordered_set<T> visited;
        std::deque<Node<T>> d = { nodes[0] };

        while (!d.empty())
        {
            if (visited.find(d[0].data) == visited.end())
            {
                result.push_back(d[0].data);
                visited.insert(d[0].data);

                for (auto &node : d[0].adjacencyList)
                {
                    if (visited.find(node->data) == visited.end())
                    {
                        d.push_back(*node);
                    }
                }
            }

            d.pop_front();
        }

        return result;
    }
    
    bool find(const T &value) const
    {
        const std::vector<T> nodes = depthFirstTraversal();
        return std::find(nodes.cbegin(), nodes.cend(), value) != nodes.cend();
    }

private:

    void depthFirstImpl(const Node<T> &n, std::vector<T> &values, std::unordered_set<T> &visited) const
    {
        visited.insert(n.data);
        values.push_back(n.data);

        for (const auto &child : n.adjacencyList)
        {
            if (visited.find(child->data) == visited.end())
            {
                depthFirstImpl(*child, values, visited);
            }
        }
    }

    std::vector<Node<T>> nodes;
};

void testGraph()
{
    std::vector<Node<int>> nodes;
    Graph<int> gEmpty{nodes};
    assert(gEmpty.depthFirstTraversal().empty());
    assert(gEmpty.breadthFirstTraversal().empty());
    Node<int> n1{ 1 };
    Node<int> n2{ 2 };
    Node<int> n3{ 3 };
    Node<int> n4{ 4 };
    Node<int> n5{ 5 };

    std::vector<Node<int>*> adj = { &n2 };
    n1.setAdjacencyList(adj);
    adj = { &n1, &n3, &n4 };
    n2.setAdjacencyList(adj);
    adj = { &n2, &n4 };
    n3.setAdjacencyList(adj);
    adj = { &n2, &n3, &n5 };
    n4.setAdjacencyList(adj);
    adj = { &n4 };
    n5.setAdjacencyList(adj);

    nodes = { n1, n2, n3, n4, n5 };
    Graph<int> g{ nodes };

    nodes = g.getNodes();
    assert(nodes[0].data == 1);
    assert(nodes[0].getAdjacencyList() == std::vector<int>({2}));
    assert(nodes[1].data == 2);
    assert(nodes[1].getAdjacencyList() == std::vector<int>({1, 3 ,4 }));
    assert(nodes[2].data == 3);
    assert(nodes[2].getAdjacencyList() == std::vector<int>({ 2, 4 }));
    assert(nodes[3].data == 4);
    assert(nodes[3].getAdjacencyList() == std::vector<int>({ 2, 3, 5 }));

    assert(g.depthFirstTraversal() == std::vector<int>({ 1, 2, 3, 4, 5 }));
    assert(g.breadthFirstTraversal() == std::vector<int>({1, 2, 3, 4, 5}));
}

void testGraphDataTypes()
{
    Node<double> n1{ 1.1 };
    Node<double> n2{ 2.2 };
    Node<double> n3{ 3.3 };
    Node<double> n4{ 4.4 };
    Node<double> n5{ 5.5 };

    std::vector<Node<double>*> adj = { &n2 };
    n1.setAdjacencyList(adj);
    adj = { &n1, &n3, &n4 };
    n2.setAdjacencyList(adj);
    adj = { &n2, &n4 };
    n3.setAdjacencyList(adj);
    adj = { &n2, &n3, &n5 };
    n4.setAdjacencyList(adj);
    adj = { &n4 };
    n5.setAdjacencyList(adj);

    std::vector<Node<double>> nodes = { n1, n2, n3, n4, n5 };
    Graph<double> g{ nodes };

    assert(g.find(2.2));
    assert(!g.find(6.6));
}
