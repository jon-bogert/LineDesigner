#include "ConnectionGraph.h"

bool ConnectionGraph::HasConnection(const uint32_t idA, const uint32_t idB) const
{
    auto iterA = m_nodes.find(idA);
    if (iterA == m_nodes.end())
        return false;

    return iterA->second.connections.find(idB) != iterA->second.connections.end();
}

void ConnectionGraph::CreateConnection(const uint32_t idA, const uint32_t idB)
{
    Node& nodeA = m_nodes[idA];
    Node& nodeB = m_nodes[idB];

    nodeA.connections.insert(idB);
    nodeB.connections.insert(idA);
}

void ConnectionGraph::RemoveConnection(const uint32_t idA, const uint32_t idB)
{
    Node& nodeA = m_nodes[idA];
    Node& nodeB = m_nodes[idB];

    nodeA.connections.erase(idB);
    nodeB.connections.erase(idA);

    if (nodeA.connections.empty())
    {
        m_nodes.erase(idA);
    }
    if (nodeB.connections.empty())
    {
        m_nodes.erase(idB);
    }
}

void ConnectionGraph::ForEach(const std::function<void(const uint32_t, const uint32_t, void*)>& visitor, void* userData)
{
    ResetVisitFlags();
    for (auto& nodePair : m_nodes)
    {
        nodePair.second.visited = true;
        for (const uint32_t dest : nodePair.second.connections)
        {
            if (m_nodes[dest].visited)
                continue;

            visitor(nodePair.first, dest, userData);
        }
    }
}

void ConnectionGraph::ResetVisitFlags()
{
    for (auto& nodePair : m_nodes)
    {
        nodePair.second.visited = false;
    }
}
