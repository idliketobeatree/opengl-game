#pragma once
#include "BlockType.hpp"

struct Block {
    BlockType type;
    float influence;

    operator bool() const {
        return type.ID;
    }
};
const static BlockType AIR{0};