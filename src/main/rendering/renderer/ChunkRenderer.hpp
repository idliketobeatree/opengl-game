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
    Vector3f *gradList3f;

    static uint16_t *vertListu;
    static uint16_t *edgeTable;
    static uint8_t **triTable;
public:
    Chunk *chunk;
    VertexArray vertices;

    Vector3f lightPos;

    VAO vao;
    VBO vbo;

    float noiseThreshold;

    ChunkRenderer(Chunk *chunk, float noiseThreshold);

    void generateMesh();

    void setupRender();
    void render(double dt);

    void genMesh();

    Vector3f vertexInterp(Vector3f a, Vector3f b, float ia, float ib) const;
//    Vector3f gradInterp(uint16_t i, Vector3f offset) const;
    Vector3f gradInterp(uint16_t i, Vector3f offset) const;
    Vector3f gradInterpNoise(Vector3f pos) const;
//    float gradInterp(float a, float b, float c) const;
};