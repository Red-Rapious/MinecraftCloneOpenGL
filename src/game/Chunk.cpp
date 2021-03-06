#include "Chunk.hpp"
#include "Constants.hpp"
#include "../graphics/Utility.hpp"
#include <iostream>
#include <vector>
#include <memory>

Chunk::Chunk(const ChunkCoord& coord)
	: m_coord(coord), m_blocksArray(), m_vertexIndexBufferCouple()
{
	// Initialise the chunk to an empty cube of air
	// TODO: change to a less ressource consuming method
	DeleteAllBlocks();
}

Chunk::~Chunk()
{
	DeleteBuffers();
}

/* VERTEX AND INDEX BUFFERS PART */
static float getTextureOriginY(const BlockType& blockType)
{
	return (float)(BLOCK_TYPES_TEXTURES_NUMBER - (int)(blockType));
}

static float getTextureCoordX(const FaceType& faceType)
{
	return ((float)faceType);
}

void Chunk::AddVertexToVertexBuffer(const glm::vec3& vertexCoord, const glm::vec2& textureCoord, const FaceType& faceType)
{
	// Add two vectors to a the vertex buffer of the chunk
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.x);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.y);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.z);

	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)textureCoord.x);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)textureCoord.y);

	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)faceType);
}


void Chunk::AddFaceToCouple(const FaceType& faceType, const glm::vec3& blockCoord, const BlockType& blockType)
{	// Called for each face that's going to be rendered
	// Add the proper vertex coordinates, textures UVs, and index count to the buffers

	glm::vec3 vertexCoord[4] = {}; // one for each vertex of each face
	glm::vec2 textureCoord[4] = {};
	
	unsigned int indices[6] = { 0,1,2,0,2,3 };

	switch (faceType)
	{
	default:
		break;
	case FaceType::LEFT:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(1, 0); // D
		textureCoord[1] = glm::vec2(0, 0); // A
		textureCoord[2] = glm::vec2(0, 1); // H
		textureCoord[3] = glm::vec2(1, 1); // E
		break;
		
	case FaceType::RIGHT:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[3] = glm::vec3(0.5f, 0.5f, -0.5f); // G


		textureCoord[0] = glm::vec2(1, 0); // B
		textureCoord[1] = glm::vec2(0, 0); // C
		textureCoord[2] = glm::vec2(0, 1); // F
		textureCoord[3] = glm::vec2(1, 1); // G
		break;
		
		
	
	case FaceType::BACK:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, -0.5f); // H


		textureCoord[0] = glm::vec2(1, 0); // A
		textureCoord[1] = glm::vec2(0, 0); // B
		textureCoord[2] = glm::vec2(0, 1); // G
		textureCoord[3] = glm::vec2(1, 1); // H
		break;
		

	case FaceType::FRONT:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, 0.5f); // E
		vertexCoord[3] = glm::vec3(0.5f, 0.5f, 0.5f); // F


		textureCoord[0] = glm::vec2(1, 0); // C
		textureCoord[1] = glm::vec2(0, 0); // D
		textureCoord[2] = glm::vec2(0, 1); // E
		textureCoord[3] = glm::vec2(1, 1); // F
		break;
		

	case FaceType::UP:
		vertexCoord[0] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[1] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(0, 1); // H
		textureCoord[1] = glm::vec2(1, 1); // G
		textureCoord[2] = glm::vec2(1, 0); // F
		textureCoord[3] = glm::vec2(0, 0); // E
		break;

	case FaceType::DOWN:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[3] = glm::vec3(0.5f, -0.5f, 0.5f); // C


		textureCoord[0] = glm::vec2(0, 0); // B
		textureCoord[1] = glm::vec2(0, 1); // A
		textureCoord[2] = glm::vec2(1, 1); // D
		textureCoord[3] = glm::vec2(1, 0); // C
		break;
	}



	float textureOriginX = 0.0f;
	float textureOriginY = 0.0f;
	for (int i = 0; i < sizeof(vertexCoord) / sizeof(vertexCoord[0]); i++)
	{
		// Add the block coord (in world space) to the vertex
		vertexCoord[i].x += blockCoord.x;
		vertexCoord[i].y += blockCoord.y;
		vertexCoord[i].z += blockCoord.z;

		// Add chunk coord in world space
		vertexCoord[i] += glm::vec3(m_coord.idx * CHUNK_X_BLOCK_COUNT, 0, (m_coord.idz) * CHUNK_Z_BLOCK_COUNT);
		
		// Add the texture origin on the texture map
		textureOriginY = getTextureOriginY(blockType); // TODO: dont compute it for every face, only for every block
		textureOriginX = getTextureCoordX(faceType);
		textureCoord[i].x = (textureOriginX + textureCoord[i].x) / 6;
		textureCoord[i].y = (textureOriginY + textureCoord[i].y) / BLOCK_TYPES_TEXTURES_NUMBER;

		// Add vertex to the vertex buffer
		AddVertexToVertexBuffer(vertexCoord[i], textureCoord[i], faceType);
	}

	// Add the good indices to the index buffer
	for (unsigned int i = 0; i < 6; i++)
	{
		m_vertexIndexBufferCouple.m_indexBuffer.push_back(indices[i]+ m_vertexIndexBufferCouple.m_indexCount);
		
	}
	m_vertexIndexBufferCouple.m_indexCount+=4;
}



/* STANDARD FUNCTIONS PART */
void Chunk::SetBlockType(const glm::vec3& blockPosition, const BlockType& type)
{
	if (!(blockPosition.x >= 0 && blockPosition.x < CHUNK_X_BLOCK_COUNT && blockPosition.y >= 0 && blockPosition.y < CHUNK_Y_BLOCK_COUNT && blockPosition.z >= 0 && blockPosition.z < CHUNK_Z_BLOCK_COUNT))
		std::cout << "[SetBlockType] Impossible to set this block, the coordinates are incorrect: " << blockPosition.x << ", " << blockPosition.y << ", " << blockPosition.z << "\n";
	else
		m_blocksArray[(unsigned int)blockPosition.x][(unsigned int)blockPosition.y][(unsigned int)blockPosition.z] = type;
}

void Chunk::SetBlockInterChunk(const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap, const glm::vec3& blockPosition, const BlockType& type, const bool& regenBuffers)
{
	int x = blockPosition.x; 
	int y = blockPosition.y;
	int z = blockPosition.z;
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_X_BLOCK_COUNT && y < CHUNK_Y_BLOCK_COUNT && z < CHUNK_Z_BLOCK_COUNT)
		// if the block is in this chunk
		this->SetBlockType(blockPosition, type);
	else // if the block is in another chunk
	{
		int chunkCX = x/CHUNK_X_BLOCK_COUNT;
		int chunkCZ = z / CHUNK_Z_BLOCK_COUNT;
		if (x < 0)
			chunkCX--;
		if (z < 0)
			chunkCZ--;
		ChunkCoord otherChunkCoord(chunkCX,chunkCZ);

		if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if the other chunk exists
		{
			int t1 = x - chunkCX * 16;
			int t2 = z - chunkCZ * 16;
			chunksUMap.at(otherChunkCoord)->SetBlockType(glm::vec3(x-chunkCX*16, y, z-chunkCZ*16), type);
			if (regenBuffers)
			{
				chunksUMap.at(otherChunkCoord)->ListAllFacesToRender(chunksUMap);
				chunksUMap.at(otherChunkCoord)->GenerateBuffers();
			}
		}
	}
}


void Chunk::FillPlaneWithBlocks(const unsigned int& height, const BlockType& type)
{	// Debug function
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT ; x++)
	{
		for (unsigned int z = 0 ; z < CHUNK_Z_BLOCK_COUNT; z++)
		{
			m_blocksArray[x][height][z] = type;
		}
	}

}

void Chunk::DeleteAllBlocks()
{	// Clear the array, and push back 16*16*256 blocks of air
	m_blocksArray = {};
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		std::vector<std::vector<BlockType>> vect;
		m_blocksArray.push_back(vect);
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			std::vector<BlockType> vect(CHUNK_Z_BLOCK_COUNT, BlockType::NONE);
			m_blocksArray[x].push_back(vect);
		}
	}
}

unsigned int Chunk::GetNumberOfNonAirBlocks(const bool& out) const
{	// Debug function
	if (m_blocksArray.size() == 0)
	{
		std::cout << "[Chunk Blocks Error] The array of blocks is empty, cannot calculate any blocks\n";
		return 0;
	}
	/* Debug func that count the number of "true" blocks */
	unsigned int count = 0;

	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				if (m_blocksArray[x][y][z] != BlockType::NONE)
					count++;
			}
		}
	}

	if (out)
		std::cout << "Number of non air blocks: " << count << "\n";

	return count;
}

//enum ChunkRelativePosition
//{
//	NORTH, SOUTH, EAST, WEST
//};

void Chunk::Generate(const siv::PerlinNoise& noise, const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap)
{	// Called ONCE on chunk generation: create the terrain, trees, etc...
	// Trees coordinates generation
	// Trees coordinates are calculated before terrain generation, to be able to get the y coordinate of the tree
	const unsigned int minNbTrees = 0;
	const unsigned int maxNbTrees = 3;
	const unsigned int numberOfTrees = rand() % (maxNbTrees + 1) + minNbTrees;

	std::vector<glm::vec3> treesCoordinates;

	//bool isChunkGenerated[4] = { false };
	//isChunkGenerated[NORTH] = chunksUMap.find(ChunkCoord(m_coord.idx, m_coord.idz - 1)) != chunksUMap.end();
	//isChunkGenerated[SOUTH] = chunksUMap.find(ChunkCoord(m_coord.idx, m_coord.idz + 1)) != chunksUMap.end();
	//isChunkGenerated[EAST] = chunksUMap.find(ChunkCoord(m_coord.idx + 1, m_coord.idz)) != chunksUMap.end();
	//isChunkGenerated[WEST] = chunksUMap.find(ChunkCoord(m_coord.idx - 1, m_coord.idz)) != chunksUMap.end();

	for (unsigned int i = 0 ; i < numberOfTrees ; i++)
	{
		// Calculate the coordinates of the tree
		treesCoordinates.push_back(glm::vec3(2 + (rand() % 12), -1, 2 + (rand() % 12)));
	}


	// Terrain generation
	const float reverseScale = 50.0f;
	const float heightFactor = 0.35f;
	const float sandFactor = 0.275f * heightFactor;
	const glm::vec2 noiseMapStartingPoint(0,0);
	ASSERT(heightFactor <= 1.0f);

	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int z = 0; z < CHUNK_X_BLOCK_COUNT; z++)
		{
			unsigned int ymax = (unsigned int)(noise.normalizedOctaveNoise2D_0_1((int)(m_coord.idx * CHUNK_X_BLOCK_COUNT + x) / reverseScale, (int)(m_coord.idz * CHUNK_Z_BLOCK_COUNT + z) / reverseScale, 3) * CHUNK_Y_BLOCK_COUNT * heightFactor);
			
			for (unsigned int y = 0; y <= ymax; y++)
			{
				if (y == ymax)
				{
					if (y <= CHUNK_Y_BLOCK_COUNT*sandFactor)
					{
						SetBlockType(glm::vec3(x, y, z), BlockType::SAND);
					}
					else
						SetBlockType(glm::vec3(x,y,z), BlockType::GRASS);
				}
				else if (y==0)
					SetBlockType(glm::vec3(x, y, z), BlockType::BEDROCK);
				else if (y >= ymax - 2)
				{
					if (y <= CHUNK_Y_BLOCK_COUNT * sandFactor)
					{
						SetBlockType(glm::vec3(x, y, z), BlockType::SAND);
					}
					else
						SetBlockType(glm::vec3(x, y, z), BlockType::DIRT);
				}
				else
				{
					SetBlockType(glm::vec3(x, y, z), BlockType::STONE);
				}
			}

			for (unsigned int w = 0 ; w<treesCoordinates.size() ; w++)
			{
				if (treesCoordinates[w] == glm::vec3(x, -1, z))
					treesCoordinates[w].y = ymax;
			}
		}
	}

	// Trees generation
	for (unsigned int i = 0; i < treesCoordinates.size(); i++)
	{
		if (treesCoordinates[i].y < CHUNK_Y_BLOCK_COUNT-8)
			CreateTree(glm::vec3(treesCoordinates[i]));
	}
}	

void Chunk::CreateTree(const glm::vec3& coords)
{	// Create a tree, where the specified coords are the coords of the lower block of the trunk
	const unsigned int trunkHeight = 5;

	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			SetBlockType(glm::vec3(coords.x+i, coords.y + 3, coords.z+j), BlockType::OPAQUE_LEAFS);
			SetBlockType(glm::vec3(coords.x +i, coords.y + 4, coords.z +j), BlockType::OPAQUE_LEAFS);
		}
	}

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			SetBlockType(glm::vec3(coords.x + i, coords.y + 5, coords.z + j), BlockType::OPAQUE_LEAFS);
		}
	}

	SetBlockType(glm::vec3(coords.x, coords.y + 6, coords.z), BlockType::OPAQUE_LEAFS);
	SetBlockType(glm::vec3(coords.x -1, coords.y + 6, coords.z), BlockType::OPAQUE_LEAFS);
	SetBlockType(glm::vec3(coords.x +1, coords.y + 6, coords.z), BlockType::OPAQUE_LEAFS);
	SetBlockType(glm::vec3(coords.x, coords.y + 6, coords.z +1), BlockType::OPAQUE_LEAFS);
	SetBlockType(glm::vec3(coords.x, coords.y + 6, coords.z -1), BlockType::OPAQUE_LEAFS);

	for (unsigned int i = 0; i < trunkHeight; i++)
		SetBlockType(glm::vec3(coords.x, coords.y + i, coords.z), BlockType::LOG);
}

void Chunk::GenerateBuffers()
{	// Called everytime the geometry of the chunk changes

	//DeleteBuffers();
	GLCall(glGenBuffers(1, &m_vertexBufferID));
	GLCall(glGenBuffers(1, &m_indexBufferID));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID));
}

void Chunk::DeleteBuffers()
{
	GLCall(glDeleteBuffers(1, &m_vertexBufferID));
	GLCall(glDeleteBuffers(1, &m_indexBufferID));
}


void Chunk::RenderChunk(const VertexArray& vao, const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap)
{	// Called once every frame, make the draw call for the chunk
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*m_vertexIndexBufferCouple.m_vertexBuffer.size(), m_vertexIndexBufferCouple.m_vertexBuffer.data(), GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndexBufferCouple.m_indexBuffer.size() * sizeof(unsigned int), m_vertexIndexBufferCouple.m_indexBuffer.data(), GL_STATIC_DRAW));



	VertexBufferLayout bufferLayout;

	bufferLayout.Push<float>(3); // add 3 floats for the vertex positions
	bufferLayout.Push<float>(2); // add 2 floats for the texture coords
	bufferLayout.Push<float>(1); // add 1 float for the face type
	vao.AddBuffer(bufferLayout);

	GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)m_vertexIndexBufferCouple.m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr));
}

/* FACE TO RENDER ALGORITHM PART */
void Chunk::ListAllFacesToRender(const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap)
{	// Called once everytime the chunk or a neighboor is changing
	// For each block, check for each face is there's a non-air block next to it: if so, it doesn't render the face
	
	const bool RENDER_UNGEN_CHUNKS_FACES = true;
	const bool RENDER_BOTTOM_CHUNK_FACES = true;

	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				// For each block
				if (m_blocksArray[x][y][z] != BlockType::NONE)
				{
					bool renderFace[6] = { false };

					/* For each face, check if its supposed to be rendered */
					// BACK
					if (z == 0)
					{
						ChunkCoord otherChunkCoord(m_coord.idx, m_coord.idz - 1); // coherent with the opengl z axis
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							if (chunksUMap.at(otherChunkCoord)->m_blocksArray.size() != 0)
							{
								// render the face if the blocks that correspond on the other chunk is empty
								renderFace[(int)FaceType::BACK] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT - 1)) == BlockType::NONE;
							}
							else
								std::cout << "[InterChunkAccess Error] Unable to access to a properly loaded chunk at coordinates: " << otherChunkCoord.idx << ", " << otherChunkCoord.idz << "\n";
						}
						else
							renderFace[(int)FaceType::BACK] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::BACK] = m_blocksArray[x][y][z-1] == BlockType::NONE;
					}

					// FRONT
					if (z == CHUNK_Z_BLOCK_COUNT-1)
					{
						ChunkCoord otherChunkCoord(m_coord.idx, m_coord.idz + 1);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							if (chunksUMap.at(otherChunkCoord)->m_blocksArray.size() != 0)
							{
								// render the face if the blocks that correspond on the other chunk is empty
								renderFace[(int)FaceType::FRONT] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(x, y, 0)) == BlockType::NONE;
							}
							else
								std::cout << "[InterChunkAccess Error] Unable to access to a properly loaded chunk at coordinates: " << otherChunkCoord.idx << ", " << otherChunkCoord.idz << "\n";
						}
						else
							renderFace[(int)FaceType::FRONT] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::FRONT] = m_blocksArray[x][y][z + 1] == BlockType::NONE;
					}

					// RIGHT
					if (x == CHUNK_X_BLOCK_COUNT-1)
					{
						ChunkCoord otherChunkCoord(m_coord.idx + 1, m_coord.idz);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							renderFace[(int)FaceType::RIGHT] = (chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(0, y, z)) == BlockType::NONE);
						}
						else
						{
							renderFace[(int)FaceType::RIGHT] = RENDER_UNGEN_CHUNKS_FACES;
						}
					}
					else
					{
						renderFace[(int)FaceType::RIGHT] = m_blocksArray[x + 1][y][z] == BlockType::NONE;
					}

					// LEFT
					if (x == 0)
					{
						ChunkCoord otherChunkCoord(m_coord.idx - 1, m_coord.idz);
						
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							if (chunksUMap.at(otherChunkCoord)->m_blocksArray.size() != 0) // here, this condition isn't supposed to be true
							{
								// render the face if the blocks that correspond on the other chunk is empty
								renderFace[(int)FaceType::LEFT] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(CHUNK_X_BLOCK_COUNT-1, y, z)) == BlockType::NONE;
							}
							else
								std::cout << "[InterChunkAccess Error] Unable to access to a properly loaded chunk at coordinates: " << otherChunkCoord.idx << ", " << otherChunkCoord.idz << "\n";
						}
						else
							renderFace[(int)FaceType::LEFT] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::LEFT] = m_blocksArray[x - 1][y][z] == BlockType::NONE;
					}

					// UP
					if (y == CHUNK_Y_BLOCK_COUNT-1)
					{
						renderFace[(int)FaceType::UP] = true;
					}
					else
					{
						renderFace[(int)FaceType::UP] = m_blocksArray[x][y + 1][z] == BlockType::NONE;
					}

					// DOWN
					if (y == 0)
					{
						renderFace[(int)FaceType::DOWN] = RENDER_BOTTOM_CHUNK_FACES;
					}
					else
					{
						renderFace[(int)FaceType::DOWN] = m_blocksArray[x][y - 1][z] == BlockType::NONE;
					}


					/* Render each face supposed to be rendered */
					for (unsigned int i = 0; i < 6; i++)
					{
						if (renderFace[i])
						{
							AddFaceToCouple((FaceType)i, glm::vec3(x,y,z), m_blocksArray[x][y][z]);
						}
					}
				}
			}
		}
	}
}