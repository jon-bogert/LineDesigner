#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

enum class ConnectionType
{
	Line,
	ArcLine
};

struct ConnectionInfo
{
	ConnectionType type = ConnectionType::Line;

	float radius = 0.f;
	int segmentCount = 32;
	bool invertArc = false;
};

using ConnectionCallback = std::function<void(const uint32_t, const uint32_t, ConnectionInfo&, void*)>;

class ConnectionGraph
{
public:

	bool HasConnection(const uint32_t idA, const uint32_t idB) const;
	void CreateConnection(const uint32_t idA, const uint32_t idB);
	void RemoveConnection(const uint32_t idA, const uint32_t idB);

	bool HasID(uint32_t id);
	std::vector<uint32_t> GatherConnectedIDs(uint32_t id);

	void ForEach(const ConnectionCallback& visitor, void* userData = nullptr);
	ConnectionInfo& operator[](std::pair<uint32_t, uint32_t> idPair);

private:
	uint64_t FindConnectionID(const uint32_t idA, uint32_t idB);
	static uint64_t CreateConnectionID(const uint32_t idA, uint32_t idB);
	static void BreakConnectionID(const uint64_t id, uint32_t& out_idA, uint32_t& out_idB);

	std::unordered_map<uint64_t, ConnectionInfo> m_connections;
};