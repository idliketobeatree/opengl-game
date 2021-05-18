#include <math/math.hpp>

struct BlockType {
    uint_fast16_t ID;

    operator uint_fast16_t() const {
        return ID;
    }
    operator bool() const {
        return ID;
    }
};