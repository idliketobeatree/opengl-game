#include "GameRenderer.hpp"

#include <util/math/Noise.hpp>
#include <world/block/blocks.hpp>

GameRenderer::GameRenderer(): chunkRenderer(new Chunk(), 0.5f) {

}

void GameRenderer::render(double dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    chunkRenderer.render(dt);
}

void GameRenderer::setupRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    Noise noise;

    Vector3u8 pos = {0,0,0};
    uint32_t i    = 0;
    for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++, i += CHUNK_Z) {
        for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++, i += CHUNK_Y) {
            for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++, i += CHUNK_X) {
                float noiseV = noise.noise3f(pos.x / 40.0f, pos.y / 10.0f, pos.z / 40.0f);
                if (noiseV > chunkRenderer.noiseThreshold)
                    chunkRenderer.chunk->setBlock(i, Block{DIRT, noiseV});
                else
                    chunkRenderer.chunk->setBlock(i, Block{AIR, noiseV});
            }
            i -= CHUNK_Y;
        }
        i -= CHUNK_Z;
    }
    chunkRenderer.setupRender();
}