#include "Chunk.hpp"

Chunk::Chunk(): Chunk({0,0,0}) {}
Chunk::Chunk(Vector3i position): position(position) {}

Block Chunk::getBlock(Vector3u8 pos) const {
    return this->blocks[getIndex(pos)];
}
Block Chunk::getBlock(uint32_t index) const {
    return this->blocks[index];
}

void Chunk::setBlock(Vector3u8 pos, Block block) {
    this->blocks[this->getIndex(pos)] = block;
}
void Chunk::setBlock(uint32_t index, Block block) {
    this->blocks[index] = block;
}

uint32_t Chunk::getIndex(Vector3u8 pos) {
    return (pos.z * CHUNK_SIZE2) + (pos.y * CHUNK_SIZE) + pos.x;
}
bool Chunk::outOfBounds(uint32_t index) {
    return index > CHUNK_SIZE3-1;
}