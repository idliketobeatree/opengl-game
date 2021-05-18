#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/world/chunk/Chunk.hpp>

class ChunkRenderer: public Renderer {
protected:
    ShaderProgram program;
    UniformLoc vpLoc;
    Vector3f *vertList;

    static uint16_t *edgeTable;
    static uint8_t **triTable;
public:
    Chunk *chunk;
    VertexArray vertices;

    VAO vao;
    VBO vbo;
    EBO ebo;

    ChunkRenderer(Chunk *chunk);

    void generateMesh();

    void setupRender();
    void render(double dt);

    void genMesh();

    static Vector3f vertexInterp(Vector3f a, Vector3f b, float ia, float ib);
};