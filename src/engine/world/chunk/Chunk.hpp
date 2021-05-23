#pragma once

#include <math/math.hpp>
#include <engine/world/block/Block.hpp>

#define CHUNK_SIZE 40
#define CHUNK_SIZE2 1600
#define CHUNK_SIZE3 64000

#define CHUNK_X Chunk::getIndex({1,0,0})
#define CHUNK_Y Chunk::getIndex({0,1,0})
#define CHUNK_Z Chunk::getIndex({0,0,1})

class Chunk {
public:
    Vector3i position;
    Block blocks[CHUNK_SIZE3];

    Chunk();
    Chunk(Vector3i position);

    Block getBlock(uint16_t index) const;
    Block getBlock(Vector3u8 pos) const;

    void setBlock(uint16_t index, Block block);
    void setBlock(Vector3u8 pos, Block block);

    static uint16_t getIndex(Vector3u8 pos);
    static bool outOfBounds(uint16_t index);
};
