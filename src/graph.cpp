#include "../include/graph.h"
#include <iostream>
Graph::Graph(std::size_t countOfVertices, bool isDirected) :
    m_countOfVertices(countOfVertices),
    m_isDirected(isDirected)
{
    this->m_adjList.resize(this->m_countOfVertices);
}

void Graph::addEdge(std::size_t u, std::size_t v)
{
    if(u >= 0 && v >= 0 && u < this->m_countOfVertices && v < this->m_countOfVertices)
    {
        this->m_adjList[u].push_back(v);
        if(!this->m_isDirected)
        {
            this->m_adjList[v].push_back(u);
        }
    }
}

void Graph::removeEdge(std::size_t u, std::size_t v)
{
    if(u >= 0 && v >= 0 && u < this->m_countOfVertices && v < this->m_countOfVertices)
    {
        this->m_adjList[u].remove(v);

        if(!this->m_isDirected)
        {
            this->m_adjList[v].remove(u);
        }
    }
}

void Graph::printGraph()
{
    for(std::size_t u = 0; u < this->m_countOfVertices; ++u)
    {
        std::cout << "Vertex: " << u << ": ";
        for(std::size_t v : this->m_adjList[u])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Graph::dfs(std::size_t u)
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    dfs_helper(u, visited);
    std::cout << std::endl;
}

void Graph::dfs_helper(std::size_t u, std::vector<bool>& visited)
{
    visited[u] = true;
    std::cout << u << " ";
    for(const auto& u : this->m_adjList)
    {
        for(std::size_t v : u)
        {
            if(!visited[v])
            {
                dfs_helper(v, visited);
            }
        }
    }
}

void Graph::bfs(std::size_t u)
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    std::queue<std::size_t> q;
    q.push(u);
    visited[u] = true;
    while(!q.empty())
    {
        std::size_t current = q.front();
        std::cout << current << " ";
        q.pop();

        for(std::size_t v : this->m_adjList[current])
        {
            if(!visited[v])
            {
                q.push(v);
                visited[v] = true;
            }
        }
    }
    std::cout << std::endl;
}

void Graph::transpose()
{
    std::vector<std::list<std::size_t>> transposedGraph(this->m_countOfVertices);
    for(std::size_t u = 0; u < this->m_countOfVertices; ++u)
    {
        for(std::size_t v: this->m_adjList[u])
        {
            transposedGraph[v].push_back(u);
            if(!this->m_isDirected) transposedGraph[u].push_back(v);
        }
    }
    this->m_adjList = transposedGraph;
}

std::vector<std::size_t> Graph::getShortestPath(std::size_t u, std::size_t v)
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    std::vector<int> parents(this->m_countOfVertices, -1);
    std::queue<std::size_t> q;
    q.push(u);
    visited[u] = true;
    while(!q.empty())
    {
        std::size_t current = q.front();
        q.pop();
        
        if(current == v) break;

        for(std::size_t i : this->m_adjList[current])
        {
            if(!visited[i])
            {
                visited[i] = true;
                q.push(i);
                parents[i] = current;
            }
        }
    }

    std::vector<std::size_t> path;
    if(!visited[v]) return path;
    
    for(int at = v; at != -1; at = parents[at])
    {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::size_t Graph::getCountNodesAtAGivenLevelBFS(std::size_t u, std::size_t level)
{
    std::vector<std::size_t> levels(this->m_countOfVertices, 0);
    std::vector<bool> visited(this->m_countOfVertices, false);
    std::queue<std::size_t> q;
    q.push(u);
    visited[u] = true;
    while (!q.empty())
    {
        std::size_t current = q.front();
        q.pop();
        for(std::size_t v : this->m_adjList[current])
        {
            if(!visited[v])
            {
                visited[v] = true;
                levels[v] = levels[current] + 1;
                q.push(v);
            }
        }
    }

    std::size_t count = 0;
    for(int i = 0; i < this->m_countOfVertices; ++i)
    {
        if(levels[i] == level) ++count;
        
    }
    return count;
    
}

void Graph::dfs_helper_count_nodes_at_given_level(std::size_t node, std::size_t targetLevel, std::size_t currentLevel, std::vector<bool>& visited, std::size_t& count)
{
    visited[node] = true;
    
    if(currentLevel == targetLevel) ++count;

    for(std::size_t v : this->m_adjList[node])
    {
        if(!visited[v])
        {
            dfs_helper_count_nodes_at_given_level(v, targetLevel, currentLevel + 1, visited, count);
        }
    }
}

std::size_t Graph::getCountNodesAtAGivenLevelDFS(std::size_t u, std::size_t level)
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    std::size_t count = 0;
    this->dfs_helper_count_nodes_at_given_level(u, level, 0, visited, count);
    return count;
}

std::size_t Graph::dfs_count_paths(std::size_t u, std::size_t v, std::vector<bool>& visited)
{
    if(u == v) return 1;

    visited[u] = true;
    std::size_t pathCount = 0;

    for(int i : this->m_adjList[u])
    {
        if(!visited[i])
        {
            pathCount += dfs_count_paths(i, v, visited);
        }
    }
    visited[u] = false;
    return pathCount;
}

std::size_t Graph::getCountOfAllPossiblePaths(std::size_t u, std::size_t v)
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    return dfs_count_paths(u, v, visited);
}

bool Graph::is_cycled_undirected()
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    return dfs_helper_cycle_detection_undirected(0, visited, -1);
}

bool Graph::dfs_helper_cycle_detection_undirected(std::size_t u, std::vector<bool>& visited, std::size_t parent)
{
    visited[u] = true;
    for(std::size_t v : this->m_adjList[u])
    {
        if(v != parent)
        {
            if(visited[v]) return true;
            if(dfs_helper_cycle_detection_undirected(v, visited, u)) return true;
        }
    }
    return false;
}

bool Graph::dfs_helper_cycle_detection_directed(std::size_t u, std::vector<bool>& visited, std::vector<bool>& refStack)
{
    visited[u] = true;
    refStack[u] = true;

    for(std::size_t v : this->m_adjList[u])
    {
        if(!visited[v])
        {
            if(dfs_helper_cycle_detection_directed(v, visited, refStack))
            {
                return true;
            }
        } else if(refStack[v])
        {
            return true;
        }
    }
    refStack[u] = false;
    return false;
}

bool Graph::is_cycled_directed()
{
    std::vector<bool> visited(this->m_countOfVertices, false);
    std::vector<bool> refStack(this->m_countOfVertices, false);
    return this->dfs_helper_cycle_detection_directed(0, visited, refStack);
}
bool Graph::is_cycled()
{
    return this->m_isDirected ? this->is_cycled_directed() : this->is_cycled_undirected();
}

void Graph::dfs_helper_topSort(std::size_t u, std::vector<bool>& visited, std::vector<std::size_t>& topOrder)
{
    visited[u] = true;
    for(std::size_t v : this->m_adjList[u])
    {
        if(!visited[v])
        {
            dfs_helper_topSort(v, visited, topOrder);
        }
    }
    topOrder.push_back(u);
}

std::vector<std::size_t> Graph::topSort_dfs()
{
    std::vector<std::size_t> topOrder;
    std::vector<bool> visited(this->m_countOfVertices, false);
    for(int i = 0; i < this->m_countOfVertices; ++i)
    {
        if(!visited[i])
        {
            this->dfs_helper_topSort(i, visited, topOrder); 
        }
    }
    if(topOrder.size() != this->m_countOfVertices) throw std::runtime_error("The graph is cycled. TopSort impossible");
    std::reverse(topOrder.begin(), topOrder.end());
    return topOrder;
}

std::vector<std::size_t> Graph::topSort_Kahn()
{
    std::vector<std::size_t> inDegree(this->m_countOfVertices, 0);
    for(std::size_t u = 0; u < this->m_countOfVertices; ++u)
    {
        for(const std::size_t v : this->m_adjList[u])
        {
            ++inDegree[v];
        }
    }

    std::vector<std::size_t> topOrder;
    std::queue<std::size_t> q;
    for(int i = 0; i < this->m_countOfVertices; ++i)
    {
        if(inDegree[i] == 0)
        {
            q.push(i);
        }
    }

    while(!q.empty())
    {
        std::size_t current = q.front();
        q.pop();
        topOrder.push_back(current);

        for(std::size_t v : this->m_adjList[current])
        {
            --inDegree[v];
            if(inDegree[v] == 0)
            {
                q.push(v);
            }
        }
    }
    if(topOrder.size() != this->m_countOfVertices) throw std::runtime_error("The graph is cycled. TopSort impossible");
    return topOrder;
}