#pragma once

#include <functional>
#include <unordered_set>
#include <unordered_map>

class ConnectionGraph
{
public:
	struct Node
	{
		std::unordered_set<uint32_t> connections;
		bool visited = false;
	};

	bool HasConnection(const uint32_t idA, const uint32_t idB) const;
	void CreateConnection(const uint32_t idA, const uint32_t idB);
	void RemoveConnection(const uint32_t idA, const uint32_t idB);

	bool HasID(uint32_t id) { return m_nodes.find(id) != m_nodes.end(); };

	void ForEach(const std::function<void(const uint32_t, const uint32_t, void*)>& visitor, void* userData = nullptr);
	Node& operator[](uint32_t id) { return m_nodes[id]; };

private:

	void ResetVisitFlags();

	std::unordered_map<uint32_t, Node> m_nodes;
};