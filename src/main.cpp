#include "../include/graph.h"

int main()
{
    Graph g(6, true);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(2, 5);
    g.addEdge(2, 3);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    auto topOrder = g.topSort_dfs();
    for(std::size_t i : topOrder)
    {
        std::cout << i << "  ";
    }
    return 0;
}