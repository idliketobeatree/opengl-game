#pragma once

#include <engine/rendering/rendering.hpp>
#include <engine/world/chunk/Chunk.hpp>

class ChunkRenderer: public Renderer {
protected:
    struct Edge {
        uint8_t a,b;
    };
    enum class Dir : uint8_t {
        X, Y, Z
    };
    struct ChunkEdgeVertexCoord {
        Vector3u8 pos;
        Dir dir;

        uint32_t value() const;
        bool operator<(ChunkEdgeVertexCoord other) const;
    };

    static uint16_t *edgeTable;
    static uint8_t **triTable;

    ShaderProgram program;
    UniformLoc viewLoc, mvpLoc,viewPosLoc;

    Vector3f *vertList3f;

    ChunkEdgeVertexCoord getChunkEdgeVertexCoord(Vector3u8 pos, Edge edge) const;
    void calculateVertices(uint32_t i, uint16_t edge);
public:
    struct Vertex {
        Vector3f pos;
        Vector3f normal;
        Vector3f color;
    };

    static Edge edges[12];
    Vector3u8 *cornersV[8];
    uint32_t  *cornersU[8];

    Chunk *chunk;

    Vector3u8 V0,V1,V2,V3,V4,V5,V6,V7;
    uint32_t  v0,v1,v2,v3,v4,v5,v6,v7;

    float noiseThreshold;

    uint8_t LOD;

    Vertex *vertices;
    uint32_t verticesSize;
    uint32_t *indices;
    uint32_t indicesSize;

    VAO vao;
    VBO vbo;

    EBO ebo;


    Spotlight flashlight;
    DirectionalLight sun;

    ChunkRenderer(Chunk *chunk, float noiseThreshold);
    ~ChunkRenderer();

    void generateMesh();

    void setupRender();
    void render(double dt);

    void updateLOD(uint8_t LOD);

    Vector3f vertexInterp(Vector3f a, Vector3f b, float ia, float ib) const;
    Vector3f gradInterp(uint32_t i, Vector3f offset) const;
};