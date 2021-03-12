#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"
#include "glm/glm.hpp"
#include <memory>

class Map
{
private:
	
	//VertexIndexBufferCouple m_worldCouple;

	ChunkCoord m_playerPosition;
	std::vector<ChunkCoord> m_chunkGenerationQueue;
	void AddChunkToGenQueue(ChunkCoord chunkCoord);

	ChunkCoord ConvertPositionToChunkCoord(const glm::vec3& position);
	void RenderChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender);
	

public:
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;

	inline Map() : m_playerPosition(ChunkCoord(0, 0)), m_chunkGenerationQueue({}) {};
	void AddChunkToMap(const Chunk& chunk);
	//VertexIndexBufferCouple GetCoupleToRender();
	void RenderAllNeededChunks(VertexArray& vao, const Renderer& renderer);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord).get(); };
	bool GenerateOneChunk();
	void UpdatePlayerPosition(const glm::vec3& cameraPosition);

	std::vector<ChunkCoord> GetChunksCoordsToRender();
};