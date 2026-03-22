#include "ConnectionGraph.h"

bool ConnectionGraph::HasConnection(const uint32_t idA, const uint32_t idB) const
{
    uint64_t id1 = CreateConnectionID(idA, idB);
    uint64_t id2 = CreateConnectionID(idB, idA);

    if (m_connections.find(id1) != m_connections.end())
    {
        return true;
    }

    return m_connections.find(id2) != m_connections.end();
}

void ConnectionGraph::CreateConnection(const uint32_t idA, const uint32_t idB)
{
    uint64_t connID = CreateConnectionID(idA, idB);
    ConnectionInfo& nodeA = m_connections[connID];
}

void ConnectionGraph::RemoveConnection(const uint32_t idA, const uint32_t idB)
{
    uint64_t id1 = CreateConnectionID(idA, idB);
    uint64_t id2 = CreateConnectionID(idB, idA);

    if (m_connections.find(id1) != m_connections.end())
    {
        m_connections.erase(id1);
        return;
    }

    if (m_connections.find(id2) == m_connections.end())
        return;

    m_connections.erase(id2);
}

bool ConnectionGraph::HasID(uint32_t id)
{
    for (const auto& connPair : m_connections)
    {
        uint32_t idA, idB;
        BreakConnectionID(connPair.first, idA, idB);

        if (idA != id && idB != id)
            continue;

        return true;
    }
    return false;
}

std::vector<uint32_t> ConnectionGraph::GatherConnectedIDs(uint32_t id)
{
    std::vector<uint32_t> result;

    for (const auto& connPair : m_connections)
    {
        uint32_t idA, idB;
        BreakConnectionID(connPair.first, idA, idB);

        if (idA == id)
        {
            result.push_back(idB);
            continue;
        }
        if (idB == id)
        {
            result.push_back(idA);
        }
    }
    return result;
}

void ConnectionGraph::ForEach(const ConnectionCallback& visitor, void* userData)
{
    for (auto& nodePair : m_connections)
    {
        uint32_t idA, idB;
        BreakConnectionID(nodePair.first, idA, idB);
        visitor(idA, idB, nodePair.second, userData);
    }
}

uint64_t ConnectionGraph::FindConnectionID(const uint32_t idA, uint32_t idB)
{
    uint64_t id1 = CreateConnectionID(idA, idB);
    uint64_t id2 = CreateConnectionID(idB, idA);

    if (m_connections.find(id1) != m_connections.end())
        return id1;

    if (m_connections.find(id2) != m_connections.end())
        return id2;

    return 0;
}

uint64_t ConnectionGraph::CreateConnectionID(const uint32_t idA, uint32_t idB)
{
    uint64_t result{};
    result |= (uint64_t)(idA) << (sizeof(uint32_t) * 8);
    result |= (uint64_t)(idB);
    return result;
}

void ConnectionGraph::BreakConnectionID(const uint64_t id, uint32_t& out_idA, uint32_t& out_idB)
{
    out_idA = (uint32_t)(id >> (sizeof(uint32_t) * 8));
    out_idB = (uint32_t)(id & 0x00000000FFFFFFFF);
}

ConnectionInfo& ConnectionGraph::operator[](std::pair<uint32_t, uint32_t> idPair)
{
    uint64_t id = FindConnectionID(idPair.first, idPair.second);

    if (id != 0)
        return m_connections[id];

    return m_connections[CreateConnectionID(idPair.first, idPair.second)];
}
