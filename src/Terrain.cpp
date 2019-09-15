#include "Terrain.h"

#include <stb/stb_image.h>

Terrain::Terrain(DerpRenderer* renderer) : alloc(&renderer->allocator)
{
	const uint32_t vertexCount = PATCH_SIZE * PATCH_SIZE;
	std::vector<Vertex> vertices;
	vertices.resize(vertexCount);

	const float wx = 2.0f;
	const float wy = 2.0f;

	for (auto x = 0; x < PATCH_SIZE; x++)
	{
		for (auto y = 0; y < PATCH_SIZE; y++)
		{
			uint32_t index = (x + y * PATCH_SIZE);
			vertices[index].pos.x = x * wx + wx / 2.0f - (float)PATCH_SIZE * wx / 2.0f;
			vertices[index].pos.y = 0.0f;
			vertices[index].pos.z = y * wy + wy / 2.0f - (float)PATCH_SIZE * wy / 2.0f;
			vertices[index].texCoord = glm::vec2((float)x / PATCH_SIZE, (float)y / PATCH_SIZE) * UV_SCALE;
		}
	}

	// Calculate normals from height map using a sobel filter
	HeightMap heightMap("textures/heightmap.png", PATCH_SIZE);

	for (auto x = 0; x < PATCH_SIZE; x++)
	{
		for (auto y = 0; y < PATCH_SIZE; y++)
		{
			// Get height samples centered around current position
			float heights[3][3];
			for (auto hx = -1; hx <= 1; hx++)
			{
				for (auto hy = -1; hy <= 1; hy++)
				{
					heights[hx + 1][hy + 1] = (float)heightMap.getHeight(x + hx, y + hy);
				}
			}

			// Calculate the normal
			glm::vec3 normal;
			// Gx sobel filter
			normal.x = heights[0][0] - heights[2][0] + 2.0f * heights[0][1] - 2.0f * heights[2][1] + heights[0][2] - heights[2][2];
			// Gy sobel filter
			normal.z = heights[0][0] + 2.0f * heights[1][0] + heights[2][0] - heights[0][2] - 2.0f * heights[1][2] - heights[2][2];
			// Calculate missing up component of the normal using the filtered x and y axis
			// The first value controls the bump strength
			normal.y = 0.25f * sqrt(1.0f - normal.x * normal.x - normal.z * normal.z);

			vertices[x + y * PATCH_SIZE].normal = glm::normalize(normal * glm::vec3(2.0f, 1.0f, 2.0f));
		}
	}

	// Indices
	const uint32_t w = (PATCH_SIZE - 1);
	const uint32_t indexCount = w * w * 4;
	std::vector<uint16_t> indices;
	indices.resize(indexCount);
	for (auto x = 0; x < w; x++)
	{
		for (auto y = 0; y < w; y++)
		{
			uint32_t index = (x + y * w) * 4;
			indices[index] = (x + y * PATCH_SIZE);
			indices[index + 1] = indices[index] + PATCH_SIZE;
			indices[index + 2] = indices[index + 1] + 1;
			indices[index + 3] = indices[index] + 1;
		}
	}

	uint32_t vertexBufferSize = vertexCount * sizeof(Vertex);
	uint32_t indexBufferSize = indexCount * sizeof(uint16_t);

	vertexBuffer = std::make_unique<DerpBufferLocal>(renderer->device, renderer->commandPool, vertices, renderer->allocator);
	indexBuffer = std::make_unique<DerpBufferLocal>(renderer->device, renderer->commandPool, indices, renderer->allocator);
}

Terrain::~Terrain()
{
	vmaDestroyBuffer(*alloc, vertexBuffer->buffer, vertexBuffer->allocation);
	vmaDestroyBuffer(*alloc, indexBuffer->buffer, indexBuffer->allocation);
}

// Generate a terrain quad patch for feeding to the tessellation control shader
Terrain::HeightMap::HeightMap(std::string filename, uint32_t patchsize)
{
	pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, 0);
	std::cout << "Heightmap width=" << texWidth << " height=" << texHeight << " nrComponents=" << texChannels << std::endl;
	//gli::texture2d heightTex(gli::load(filename));
	//dim = static_cast<uint32_t>(heightTex.extent().x);
	//heightdata = new uint16_t[dim * dim];
	//heightdata = new uint8_t[texWidth * texHeight];
	//memcpy(heightdata, pixels, texWidth * texHeight * sizeof(uint8_t));
	//dim = texWidth;
	this->scale = texWidth / patchsize;
}

Terrain::HeightMap::~HeightMap()
{
	//delete[] heightdata;
}

float Terrain::HeightMap::getHeight(uint32_t x, uint32_t y)
{
	glm::ivec2 rpos = glm::ivec2(x, y) * glm::ivec2(scale);
	rpos.x = std::max(0, std::min(rpos.x, (int)texWidth - 1));
	rpos.y = std::max(0, std::min(rpos.y, (int)texHeight - 1));
	rpos /= glm::ivec2(scale);
	return *(pixels + (rpos.x + rpos.y * texWidth) * scale) / 65535.0f * 0.3;
}