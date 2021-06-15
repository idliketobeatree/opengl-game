#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/world/chunk/Chunk.hpp>

class ChunkRenderer: public Renderer {
protected:
    ShaderProgram program;
    UniformLoc viewLoc, mvpLoc,
    lightColorLoc, lightPosLoc, lightStrengthLoc,
    ambientLightColorLoc, ambientLightStrengthLoc,
    viewPosLoc;

    Vector3f *vertList3f;

    static uint16_t *edgeTable;
    static uint8_t **triTable;
public:
    Vector3f  V0f, V1f, V2f, V3f, V4f, V5f, V6f, V7f;
    Vector3u8 V0i, V1i, V2i, V3i, V4i, V5i, V6i, V7i;
    uint32_t  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7;

    Chunk *chunk;
    VertexArray vertices;

    Vector3f lightPos;

    VAO vao;
    VBO vbo;

    uint8_t LOD;

    float noiseThreshold;

    ChunkRenderer(Chunk *chunk, float noiseThreshold);

    void generateMesh();

    void setupRender();
    void render(double dt);

    void updateLOD(uint8_t LOD);

    void genMesh();

    Vector3f vertexInterp(Vector3f a, Vector3f b, float ia, float ib) const;
//    Vector3f gradInterp(uint16_t i, Vector3f offset) const;
    Vector3f gradInterp(uint32_t i, Vector3f offset) const;
    Vector3f gradInterpNoise(Vector3f pos) const;
//    float gradInterp(float a, float b, float c) const;
};