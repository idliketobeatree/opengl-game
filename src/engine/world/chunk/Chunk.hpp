#pragma once

#include <math/math.hpp>
#include <engine/world/block/Block.hpp>

#define CHUNK_SIZE 200
#define CHUNK_SIZE2 CHUNK_SIZE*CHUNK_SIZE
#define CHUNK_SIZE3 CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE

//#define CHUNK_SIZE 128
//#define CHUNK_SIZE2 16384
//#define CHUNK_SIZE3 2097152

#define CHUNK_X Chunk::getIndex({1,0,0})
#define CHUNK_Y Chunk::getIndex({0,1,0})
#define CHUNK_Z Chunk::getIndex({0,0,1})

class Chunk {
public:
    Vector3i position;
    Block blocks[CHUNK_SIZE3];

    Chunk();
    Chunk(Vector3i position);

    Block getBlock(uint32_t index) const;
    Block getBlock(Vector3u8 pos) const;

    void setBlock(uint32_t index, Block block);
    void setBlock(Vector3u8 pos, Block block);

    static uint32_t getIndex(Vector3u8 pos);
    static bool outOfBounds(uint32_t index);
};
