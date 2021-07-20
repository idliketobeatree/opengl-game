#include "ChunkRenderer.hpp"

#include <util/math/Noise.hpp>

#include <rendering/rendering.hpp>
#include <logging/logging.hpp>
#include <numeric>
#include <map>
#include <vector>

ChunkRenderer::ChunkRenderer(Chunk *chunk, float noiseThreshold): chunk(chunk), noiseThreshold(noiseThreshold),
cornersV{&V0, &V1, &V2, &V3, &V4, &V5, &V6, &V7}, cornersU{&v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7},
flashlight("flashlight"), sun("sun") {
    vertList3f = new Vector3f[12];
    updateLOD(1);
}
ChunkRenderer::~ChunkRenderer() {
    free(vertices);
    free(indices);
}

void ChunkRenderer::render(double dt) {
    glUseProgram(program);
    glBindVertexArray(vao);

    Matrix4f mvp = camera.view * camera.projection;
    sun.direction = {(float)sin(glfwGetTime()/5), (float)cos(glfwGetTime()/5), 0};
    flashlight.position = camera.position;
    flashlight.direction = camera.direction;

    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvp);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.view);
    glUniform3fv(viewPosLoc, 1, camera.position);

    flashlight.setAll();
    sun.setAll();

    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
void ChunkRenderer::setupRender() {
    Shader vert(GL_VERTEX_SHADER, "res/shaders/chunk.vert");
    Shader frag(GL_FRAGMENT_SHADER, "res/shaders/chunk.frag");

    program.createProgram();
    program.attachShader(vert);
    program.attachShader(frag);
    program.linkProgram();

    vert.deleteShader();
    frag.deleteShader();

    /// chunk

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    generateMesh();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize*sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize*sizeof(uint32_t), indices, GL_STATIC_DRAW);

    // coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // colors
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    mvpLoc = glGetUniformLocation(program, "u_mvp");
    viewLoc = glGetUniformLocation(program, "u_view");
    viewPosLoc = glGetUniformLocation(program, "u_viewPos");

    sun.getLocs(program);
    sun.ambientColor = {1, 1, 1};
    sun.ambientStrength = 0.0f;
    sun.diffuseColor = {1, 0.9, 0.8};
    sun.diffuseStrength = 0.8f;
    sun.specularColor = {1, 1, 1};
    sun.specularStrength = 1.25f;

    flashlight.getLocs(program);
    flashlight.cutoff = cos(TAU/24);
    flashlight.outerCutoff = cos(TAU/16);

    flashlight.constant  = 1.0f;
    flashlight.linear    = 0.014f;
    flashlight.quadratic = 0.0007f;

    flashlight.ambientColor = {0.96f, 0.98f, 1.0f};
    flashlight.ambientStrength = 0.0f;
    flashlight.diffuseColor = {0.96f, 0.98f, 1.0f};
    flashlight.diffuseStrength = 1.25f;
    flashlight.specularColor = {1, 1, 1};
    flashlight.specularStrength = 1.5f;
}
void ChunkRenderer::updateLOD(uint8_t LOD) {
    this->LOD = LOD;

    uint8_t ONE  = LOD;
    uint8_t ZERO = 0;

    // Update vertices from values on PolyVox https://git.tgrc.dev/TGRCDev/polyvox/commit/07784f23936811bdc4a22a43fae9f3e43293e5d9
    V0 = {ZERO, ZERO, ZERO};
    V1 = {ONE,  ZERO, ZERO};
    V2 = {ZERO, ONE,  ZERO};
    V3 = {ONE,  ONE,  ZERO};
    V4 = {ZERO, ZERO, ONE};
    V5 = {ONE,  ZERO, ONE};
    V6 = {ZERO, ONE,  ONE};
    V7 = {ONE,  ONE,  ONE};

    v0 = Chunk::getIndex(V0);
    v1 = Chunk::getIndex(V1);
    v2 = Chunk::getIndex(V2);
    v3 = Chunk::getIndex(V3);
    v4 = Chunk::getIndex(V4);
    v5 = Chunk::getIndex(V5);
    v6 = Chunk::getIndex(V6);
    v7 = Chunk::getIndex(V7);
}
void ChunkRenderer::calculateVertices(uint32_t i, uint16_t edge) {
    if (edge & (1u << 0u))
        vertList3f[0] = vertexInterp(V0, V1,
                                     chunk->getBlock(i + v0).influence,
                                     chunk->getBlock(i + v1).influence);
    if (edge & (1u << 1u))
        vertList3f[1] = vertexInterp(V0, V2,
                                     chunk->getBlock(i + v0).influence,
                                     chunk->getBlock(i + v2).influence);
    if (edge & (1u << 2u))
        vertList3f[2] = vertexInterp(V0, V4,
                                     chunk->getBlock(i + v0).influence,
                                     chunk->getBlock(i + v4).influence);
    if (edge & (1u << 3u))
        vertList3f[3] = vertexInterp(V1, V3,
                                     chunk->getBlock(i + v1).influence,
                                     chunk->getBlock(i + v3).influence);
    if (edge & (1u << 4u))
        vertList3f[4] = vertexInterp(V1, V5,
                                     chunk->getBlock(i + v1).influence,
                                     chunk->getBlock(i + v5).influence);
    if (edge & (1u << 5u))
        vertList3f[5] = vertexInterp(V2, V3,
                                     chunk->getBlock(i + v2).influence,
                                     chunk->getBlock(i + v3).influence);
    if (edge & (1u << 6u))
        vertList3f[6] = vertexInterp(V2, V6,
                                     chunk->getBlock(i + v2).influence,
                                     chunk->getBlock(i + v6).influence);
    if (edge & (1u << 7u))
        vertList3f[7] = vertexInterp(V3, V7,
                                     chunk->getBlock(i + v3).influence,
                                     chunk->getBlock(i + v7).influence);
    if (edge & (1u << 8u))
        vertList3f[8] = vertexInterp(V4, V5,
                                     chunk->getBlock(i + v4).influence,
                                     chunk->getBlock(i + v5).influence);
    if (edge & (1u << 9u))
        vertList3f[9] = vertexInterp(V4, V6,
                                     chunk->getBlock(i + v4).influence,
                                     chunk->getBlock(i + v6).influence);
    if (edge & (1u << 10u))
        vertList3f[10] = vertexInterp(V5, V7,
                                      chunk->getBlock(i + v5).influence,
                                      chunk->getBlock(i + v7).influence);
    if (edge & (1u << 11u))
        vertList3f[11] = vertexInterp(V6, V7,
                                      chunk->getBlock(i + v6).influence,
                                      chunk->getBlock(i + v7).influence);
}
void ChunkRenderer::generateMesh() {
    std::map<ChunkEdgeVertexCoord, uint32_t> map;

    // make absolutely certain we have enough memory for the indices/vertices. will be shrunk later
    // *you're* bad practice >:(
    indices = (uint32_t*)realloc(indices, CHUNK_SIZE3*15*sizeof(uint32_t));
    indicesSize = 0;
    vertices = (Vertex*)realloc(vertices, CHUNK_SIZE3*15*sizeof(Vertex));
    verticesSize = 0;

    uint32_t i = 0; // voxel index

    Vector3u8 pos{0,0,0};  // pos relative to chunk
    for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z += LOD, i += CHUNK_Z * LOD) {
        for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y += LOD, i += CHUNK_Y * LOD) {
            for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x += LOD, i += CHUNK_X * LOD) {
                uint8_t cubeIndex = 0b00000000;

                if (!Chunk::outOfBounds(i + v0) && chunk->getBlock(i + v0).type.ID != AIR.ID) cubeIndex |= (1u << 0u);
                if (!Chunk::outOfBounds(i + v1) && chunk->getBlock(i + v1).type.ID != AIR.ID) cubeIndex |= (1u << 1u);
                if (!Chunk::outOfBounds(i + v2) && chunk->getBlock(i + v2).type.ID != AIR.ID) cubeIndex |= (1u << 2u);
                if (!Chunk::outOfBounds(i + v3) && chunk->getBlock(i + v3).type.ID != AIR.ID) cubeIndex |= (1u << 3u);
                if (!Chunk::outOfBounds(i + v4) && chunk->getBlock(i + v4).type.ID != AIR.ID) cubeIndex |= (1u << 4u);
                if (!Chunk::outOfBounds(i + v5) && chunk->getBlock(i + v5).type.ID != AIR.ID) cubeIndex |= (1u << 5u);
                if (!Chunk::outOfBounds(i + v6) && chunk->getBlock(i + v6).type.ID != AIR.ID) cubeIndex |= (1u << 6u);
                if (!Chunk::outOfBounds(i + v7) && chunk->getBlock(i + v7).type.ID != AIR.ID) cubeIndex |= (1u << 7u);

                /* Cube is entirely in/out of the surface */
                if (edgeTable[cubeIndex] == 0)
                    continue;
                bool calculatedVertices = false; // whether or not we've calculated vertList3f yet

                for (uint8_t j = 1; j < triTable[cubeIndex][0] + 1; j++) {
                    const ChunkEdgeVertexCoord coord = getChunkEdgeVertexCoord(pos, edges[triTable[cubeIndex][j]]);

                    const auto it = map.find(coord);

                    if(it != map.end()) {
                        indices[indicesSize] = it->second;
                        indicesSize++;
                    } else {
                        const uint32_t index = map.size();
                        map[coord] = index;

                        if(!calculatedVertices) {
                            calculatedVertices = true;
                            calculateVertices(i, edgeTable[cubeIndex]);
                        }
                        vertices[verticesSize] = {
                                pos + vertList3f[triTable[cubeIndex][j]],
                                gradInterp(i, vertList3f[triTable[cubeIndex][j]]),
                                Vector3f{1,1,1}
                        };
                        verticesSize++;

                        indices[indicesSize] = index;
                        indicesSize++;
                    }
                }
            }
            i -= CHUNK_Y;
        }
        i -= CHUNK_Z;
    }
    vertices = (Vertex*)realloc(vertices, verticesSize*sizeof(Vertex));
    indices = (uint32_t*)realloc(indices, indicesSize*sizeof(uint32_t));

    info("Indices size:  %i", indicesSize);
    info("Vertices size: %i", verticesSize);
}
Vector3f ChunkRenderer::vertexInterp(Vector3f a, Vector3f b, float ia, float ib) const {
    return a + (this->noiseThreshold - ia) / (ib - ia) * (b-a);
}
Vector3f ChunkRenderer::gradInterp(uint32_t i, Vector3f offset) const {
    uint32_t realI = i + Chunk::getIndex({(uint8_t)round(offset.x), (uint8_t)round(offset.y), (uint8_t)round(offset.z)});

    return Vector3f::normalize({
            (Chunk::outOfBounds(realI - CHUNK_X) || Chunk::outOfBounds(realI + CHUNK_X)) ? 0 :
                    this->chunk->getBlock(realI-CHUNK_X).influence - this->chunk->getBlock(realI+CHUNK_X).influence,
            (Chunk::outOfBounds(realI - CHUNK_Y) || Chunk::outOfBounds(realI + CHUNK_Y)) ? 0 :
                    this->chunk->getBlock(realI-CHUNK_Y).influence - this->chunk->getBlock(realI+CHUNK_Y).influence,
            (Chunk::outOfBounds(realI - CHUNK_Z) || Chunk::outOfBounds(realI + CHUNK_Z)) ? 0 :
                    this->chunk->getBlock(realI-CHUNK_Z).influence - this->chunk->getBlock(realI+CHUNK_Z).influence
    });
}
Vector3u8 getCornerOffset(uint8_t corner) {
    return {(uint8_t)(corner % 2), (uint8_t)((corner / 2) % 2), (uint8_t)((corner / 4) % 2)};
}
ChunkRenderer::ChunkEdgeVertexCoord ChunkRenderer::getChunkEdgeVertexCoord(Vector3u8 pos, Edge edge) const{
    return {pos + getCornerOffset(std::min(edge.a,edge.b)), (Dir)(edge.a ^ edge.b)};
}
uint32_t ChunkRenderer::ChunkEdgeVertexCoord::value() const {
    return (pos.x << 24u) | (pos.y << 16u) | (pos.z << 8u) | (uint8_t) dir;
}
bool ChunkRenderer::ChunkEdgeVertexCoord::operator<(const ChunkEdgeVertexCoord other) const {
    return value() < other.value();
}

ChunkRenderer::Edge ChunkRenderer::edges[12] {
        {0, 1},
        {0, 2},
        {0, 4},
        {1, 3},
        {1, 5},
        {2, 3},
        {2, 6},
        {3, 7},
        {4, 5},
        {4, 6},
        {5, 7},
        {6, 7}
};
uint8_t** ChunkRenderer::triTable = new uint8_t*[256]{
        new uint8_t[]{0},
        new uint8_t[]{3, 		0,8,3},
        new uint8_t[]{3, 		0,1,9},
        new uint8_t[]{6, 		1,8,3,9,8,1},
        new uint8_t[]{3, 		3,11,2},
        new uint8_t[]{6, 		0,11,2,8,11,0},
        new uint8_t[]{6, 		1,9,0,2,3,11},
        new uint8_t[]{9, 		1,11,2,1,9,11,9,8,11},
        new uint8_t[]{3, 		1,2,10},
        new uint8_t[]{6, 		0,8,3,1,2,10},
        new uint8_t[]{6, 		9,2,10,0,2,9},
        new uint8_t[]{9, 		2,8,3,2,10,8,10,9,8},
        new uint8_t[]{6, 		3,10,1,11,10,3},
        new uint8_t[]{9, 		0,10,1,0,8,10,8,11,10},
        new uint8_t[]{9, 		3,9,0,3,11,9,11,10,9},
        new uint8_t[]{6, 		9,8,10,10,8,11},
        new uint8_t[]{3, 		4,7,8},
        new uint8_t[]{6, 		4,3,0,7,3,4},
        new uint8_t[]{6, 		0,1,9,8,4,7},
        new uint8_t[]{9, 		4,1,9,4,7,1,7,3,1},
        new uint8_t[]{6, 		8,4,7,3,11,2},
        new uint8_t[]{9, 		11,4,7,11,2,4,2,0,4},
        new uint8_t[]{9, 		9,0,1,8,4,7,2,3,11},
        new uint8_t[]{12, 		4,7,11,9,4,11,9,11,2,9,2,1},
        new uint8_t[]{6, 		1,2,10,8,4,7},
        new uint8_t[]{9, 		3,4,7,3,0,4,1,2,10},
        new uint8_t[]{9, 		9,2,10,9,0,2,8,4,7},
        new uint8_t[]{12, 		2,10,9,2,9,7,2,7,3,7,9,4},
        new uint8_t[]{9, 		3,10,1,3,11,10,7,8,4},
        new uint8_t[]{12, 		1,11,10,1,4,11,1,0,4,7,11,4},
        new uint8_t[]{12, 		4,7,8,9,0,11,9,11,10,11,0,3},
        new uint8_t[]{9, 		4,7,11,4,11,9,9,11,10},
        new uint8_t[]{3, 		9,5,4},
        new uint8_t[]{6, 		9,5,4,0,8,3},
        new uint8_t[]{6, 		0,5,4,1,5,0},
        new uint8_t[]{9, 		8,5,4,8,3,5,3,1,5},
        new uint8_t[]{6, 		9,5,4,2,3,11},
        new uint8_t[]{9, 		0,11,2,0,8,11,4,9,5},
        new uint8_t[]{9, 		0,5,4,0,1,5,2,3,11},
        new uint8_t[]{12, 		2,1,5,2,5,8,2,8,11,4,8,5},
        new uint8_t[]{6, 		1,2,10,9,5,4},
        new uint8_t[]{9, 		3,0,8,1,2,10,4,9,5},
        new uint8_t[]{9, 		5,2,10,5,4,2,4,0,2},
        new uint8_t[]{12, 		2,10,5,3,2,5,3,5,4,3,4,8},
        new uint8_t[]{9, 		10,3,11,10,1,3,9,5,4},
        new uint8_t[]{12, 		4,9,5,0,8,1,8,10,1,8,11,10},
        new uint8_t[]{12, 		5,4,0,5,0,11,5,11,10,11,0,3},
        new uint8_t[]{9, 		5,4,8,5,8,10,10,8,11},
        new uint8_t[]{6, 		9,7,8,5,7,9},
        new uint8_t[]{9, 		9,3,0,9,5,3,5,7,3},
        new uint8_t[]{9, 		0,7,8,0,1,7,1,5,7},
        new uint8_t[]{6, 		1,5,3,3,5,7},
        new uint8_t[]{9, 		7,9,5,7,8,9,3,11,2},
        new uint8_t[]{12, 		9,5,7,9,7,2,9,2,0,2,7,11},
        new uint8_t[]{12, 		2,3,11,0,1,8,1,7,8,1,5,7},
        new uint8_t[]{9, 		11,2,1,11,1,7,7,1,5},
        new uint8_t[]{9, 		9,7,8,9,5,7,10,1,2},
        new uint8_t[]{12, 		10,1,2,9,5,0,5,3,0,5,7,3},
        new uint8_t[]{12, 		8,0,2,8,2,5,8,5,7,10,5,2},
        new uint8_t[]{9, 		2,10,5,2,5,3,3,5,7},
        new uint8_t[]{12, 		9,5,8,8,5,7,10,1,3,10,3,11},
        new uint8_t[]{15, 		5,7,0,5,0,9,7,11,0,1,0,10,11,10,0},
        new uint8_t[]{15, 		11,10,0,11,0,3,10,5,0,8,0,7,5,7,0},
        new uint8_t[]{6, 		11,10,5,7,11,5},
        new uint8_t[]{3, 		7,6,11},
        new uint8_t[]{6, 		3,0,8,11,7,6},
        new uint8_t[]{6, 		0,1,9,11,7,6},
        new uint8_t[]{9, 		8,1,9,8,3,1,11,7,6},
        new uint8_t[]{6, 		7,2,3,6,2,7},
        new uint8_t[]{9, 		7,0,8,7,6,0,6,2,0},
        new uint8_t[]{9, 		2,7,6,2,3,7,0,1,9},
        new uint8_t[]{12, 		1,6,2,1,8,6,1,9,8,8,7,6},
        new uint8_t[]{6, 		10,1,2,6,11,7},
        new uint8_t[]{9, 		1,2,10,3,0,8,6,11,7},
        new uint8_t[]{9, 		2,9,0,2,10,9,6,11,7},
        new uint8_t[]{12, 		6,11,7,2,10,3,10,8,3,10,9,8},
        new uint8_t[]{9, 		10,7,6,10,1,7,1,3,7},
        new uint8_t[]{12, 		10,7,6,1,7,10,1,8,7,1,0,8},
        new uint8_t[]{12, 		0,3,7,0,7,10,0,10,9,6,10,7},
        new uint8_t[]{9, 		7,6,10,7,10,8,8,10,9},
        new uint8_t[]{6, 		6,8,4,11,8,6},
        new uint8_t[]{9, 		3,6,11,3,0,6,0,4,6},
        new uint8_t[]{9, 		8,6,11,8,4,6,9,0,1},
        new uint8_t[]{12, 		9,4,6,9,6,3,9,3,1,11,3,6},
        new uint8_t[]{9, 		8,2,3,8,4,2,4,6,2},
        new uint8_t[]{6, 		0,4,2,4,6,2},
        new uint8_t[]{12, 		1,9,0,2,3,4,2,4,6,4,3,8},
        new uint8_t[]{9, 		1,9,4,1,4,2,2,4,6},
        new uint8_t[]{9, 		6,8,4,6,11,8,2,10,1},
        new uint8_t[]{12, 		1,2,10,3,0,11,0,6,11,0,4,6},
        new uint8_t[]{12, 		4,11,8,4,6,11,0,2,9,2,10,9},
        new uint8_t[]{15, 		10,9,3,10,3,2,9,4,3,11,3,6,4,6,3},
        new uint8_t[]{12, 		8,1,3,8,6,1,8,4,6,6,10,1},
        new uint8_t[]{9, 		10,1,0,10,0,6,6,0,4},
        new uint8_t[]{15, 		4,6,3,4,3,8,6,10,3,0,3,9,10,9,3},
        new uint8_t[]{6, 		10,9,4,6,10,4},
        new uint8_t[]{6, 		4,9,5,7,6,11},
        new uint8_t[]{9, 		0,8,3,4,9,5,11,7,6},
        new uint8_t[]{9, 		5,0,1,5,4,0,7,6,11},
        new uint8_t[]{12, 		11,7,6,8,3,4,3,5,4,3,1,5},
        new uint8_t[]{9, 		7,2,3,7,6,2,5,4,9},
        new uint8_t[]{12, 		9,5,4,0,8,6,0,6,2,6,8,7},
        new uint8_t[]{12, 		3,6,2,3,7,6,1,5,0,5,4,0},
        new uint8_t[]{15, 		6,2,8,6,8,7,2,1,8,4,8,5,1,5,8},
        new uint8_t[]{9, 		9,5,4,10,1,2,7,6,11},
        new uint8_t[]{12, 		6,11,7,1,2,10,0,8,3,4,9,5},
        new uint8_t[]{12, 		7,6,11,5,4,10,4,2,10,4,0,2},
        new uint8_t[]{15, 		3,4,8,3,5,4,3,2,5,10,5,2,11,7,6},
        new uint8_t[]{12, 		9,5,4,10,1,6,1,7,6,1,3,7},
        new uint8_t[]{15, 		1,6,10,1,7,6,1,0,7,8,7,0,9,5,4},
        new uint8_t[]{15, 		4,0,10,4,10,5,0,3,10,6,10,7,3,7,10},
        new uint8_t[]{12, 		7,6,10,7,10,8,5,4,10,4,8,10},
        new uint8_t[]{9, 		6,9,5,6,11,9,11,8,9},
        new uint8_t[]{12, 		3,6,11,0,6,3,0,5,6,0,9,5},
        new uint8_t[]{12, 		0,11,8,0,5,11,0,1,5,5,6,11},
        new uint8_t[]{9, 		6,11,3,6,3,5,5,3,1},
        new uint8_t[]{12, 		5,8,9,5,2,8,5,6,2,3,8,2},
        new uint8_t[]{9, 		9,5,6,9,6,0,0,6,2},
        new uint8_t[]{15, 		1,5,8,1,8,0,5,6,8,3,8,2,6,2,8},
        new uint8_t[]{6, 		1,5,6,2,1,6},
        new uint8_t[]{12, 		1,2,10,9,5,11,9,11,8,11,5,6},
        new uint8_t[]{15, 		0,11,3,0,6,11,0,9,6,5,6,9,1,2,10},
        new uint8_t[]{15, 		11,8,5,11,5,6,8,0,5,10,5,2,0,2,5},
        new uint8_t[]{12, 		6,11,3,6,3,5,2,10,3,10,5,3},
        new uint8_t[]{15, 		1,3,6,1,6,10,3,8,6,5,6,9,8,9,6},
        new uint8_t[]{12, 		10,1,0,10,0,6,9,5,0,5,6,0},
        new uint8_t[]{6, 		0,3,8,5,6,10},
        new uint8_t[]{3, 		10,5,6},
        new uint8_t[]{3, 		10,6,5},
        new uint8_t[]{6, 		0,8,3,5,10,6},
        new uint8_t[]{6, 		9,0,1,5,10,6},
        new uint8_t[]{9, 		1,8,3,1,9,8,5,10,6},
        new uint8_t[]{6, 		2,3,11,10,6,5},
        new uint8_t[]{9, 		11,0,8,11,2,0,10,6,5},
        new uint8_t[]{9, 		0,1,9,2,3,11,5,10,6},
        new uint8_t[]{12, 		5,10,6,1,9,2,9,11,2,9,8,11},
        new uint8_t[]{6, 		1,6,5,2,6,1},
        new uint8_t[]{9, 		1,6,5,1,2,6,3,0,8},
        new uint8_t[]{9, 		9,6,5,9,0,6,0,2,6},
        new uint8_t[]{12, 		5,9,8,5,8,2,5,2,6,3,2,8},
        new uint8_t[]{9, 		6,3,11,6,5,3,5,1,3},
        new uint8_t[]{12, 		0,8,11,0,11,5,0,5,1,5,11,6},
        new uint8_t[]{12, 		3,11,6,0,3,6,0,6,5,0,5,9},
        new uint8_t[]{9, 		6,5,9,6,9,11,11,9,8},
        new uint8_t[]{6, 		5,10,6,4,7,8},
        new uint8_t[]{9, 		4,3,0,4,7,3,6,5,10},
        new uint8_t[]{9, 		1,9,0,5,10,6,8,4,7},
        new uint8_t[]{12, 		10,6,5,1,9,7,1,7,3,7,9,4},
        new uint8_t[]{9, 		3,11,2,7,8,4,10,6,5},
        new uint8_t[]{12, 		5,10,6,4,7,2,4,2,0,2,7,11},
        new uint8_t[]{12, 		0,1,9,4,7,8,2,3,11,5,10,6},
        new uint8_t[]{15, 		9,2,1,9,11,2,9,4,11,7,11,4,5,10,6},
        new uint8_t[]{9, 		6,1,2,6,5,1,4,7,8},
        new uint8_t[]{12, 		1,2,5,5,2,6,3,0,4,3,4,7},
        new uint8_t[]{12, 		8,4,7,9,0,5,0,6,5,0,2,6},
        new uint8_t[]{15, 		7,3,9,7,9,4,3,2,9,5,9,6,2,6,9},
        new uint8_t[]{12, 		8,4,7,3,11,5,3,5,1,5,11,6},
        new uint8_t[]{15, 		5,1,11,5,11,6,1,0,11,7,11,4,0,4,11},
        new uint8_t[]{15, 		0,5,9,0,6,5,0,3,6,11,6,3,8,4,7},
        new uint8_t[]{12, 		6,5,9,6,9,11,4,7,9,7,11,9},
        new uint8_t[]{6, 		10,4,9,6,4,10},
        new uint8_t[]{9, 		4,10,6,4,9,10,0,8,3},
        new uint8_t[]{9, 		10,0,1,10,6,0,6,4,0},
        new uint8_t[]{12, 		8,3,1,8,1,6,8,6,4,6,1,10},
        new uint8_t[]{9, 		10,4,9,10,6,4,11,2,3},
        new uint8_t[]{12, 		0,8,2,2,8,11,4,9,10,4,10,6},
        new uint8_t[]{12, 		3,11,2,0,1,6,0,6,4,6,1,10},
        new uint8_t[]{15, 		6,4,1,6,1,10,4,8,1,2,1,11,8,11,1},
        new uint8_t[]{9, 		1,4,9,1,2,4,2,6,4},
        new uint8_t[]{12, 		3,0,8,1,2,9,2,4,9,2,6,4},
        new uint8_t[]{6, 		0,2,4,4,2,6},
        new uint8_t[]{9, 		8,3,2,8,2,4,4,2,6},
        new uint8_t[]{12, 		9,6,4,9,3,6,9,1,3,11,6,3},
        new uint8_t[]{15, 		8,11,1,8,1,0,11,6,1,9,1,4,6,4,1},
        new uint8_t[]{9, 		3,11,6,3,6,0,0,6,4},
        new uint8_t[]{6, 		6,4,8,11,6,8},
        new uint8_t[]{9, 		7,10,6,7,8,10,8,9,10},
        new uint8_t[]{12, 		0,7,3,0,10,7,0,9,10,6,7,10},
        new uint8_t[]{12, 		10,6,7,1,10,7,1,7,8,1,8,0},
        new uint8_t[]{9, 		10,6,7,10,7,1,1,7,3},
        new uint8_t[]{12, 		2,3,11,10,6,8,10,8,9,8,6,7},
        new uint8_t[]{15, 		2,0,7,2,7,11,0,9,7,6,7,10,9,10,7},
        new uint8_t[]{15, 		1,8,0,1,7,8,1,10,7,6,7,10,2,3,11},
        new uint8_t[]{12, 		11,2,1,11,1,7,10,6,1,6,7,1},
        new uint8_t[]{12, 		1,2,6,1,6,8,1,8,9,8,6,7},
        new uint8_t[]{15, 		2,6,9,2,9,1,6,7,9,0,9,3,7,3,9},
        new uint8_t[]{9, 		7,8,0,7,0,6,6,0,2},
        new uint8_t[]{6, 		7,3,2,6,7,2},
        new uint8_t[]{15, 		8,9,6,8,6,7,9,1,6,11,6,3,1,3,6},
        new uint8_t[]{6, 		0,9,1,11,6,7},
        new uint8_t[]{12, 		7,8,0,7,0,6,3,11,0,11,6,0},
        new uint8_t[]{3, 		7,11,6},
        new uint8_t[]{6, 		11,5,10,7,5,11},
        new uint8_t[]{9, 		11,5,10,11,7,5,8,3,0},
        new uint8_t[]{9, 		5,11,7,5,10,11,1,9,0},
        new uint8_t[]{12, 		10,7,5,10,11,7,9,8,1,8,3,1},
        new uint8_t[]{9, 		2,5,10,2,3,5,3,7,5},
        new uint8_t[]{12, 		8,2,0,8,5,2,8,7,5,10,2,5},
        new uint8_t[]{12, 		9,0,1,5,10,3,5,3,7,3,10,2},
        new uint8_t[]{15, 		9,8,2,9,2,1,8,7,2,10,2,5,7,5,2},
        new uint8_t[]{9, 		11,1,2,11,7,1,7,5,1},
        new uint8_t[]{12, 		0,8,3,1,2,7,1,7,5,7,2,11},
        new uint8_t[]{12, 		9,7,5,9,2,7,9,0,2,2,11,7},
        new uint8_t[]{15, 		7,5,2,7,2,11,5,9,2,3,2,8,9,8,2},
        new uint8_t[]{6, 		1,3,5,3,7,5},
        new uint8_t[]{9, 		0,8,7,0,7,1,1,7,5},
        new uint8_t[]{9, 		9,0,3,9,3,5,5,3,7},
        new uint8_t[]{6, 		9,8,7,5,9,7},
        new uint8_t[]{9, 		5,8,4,5,10,8,10,11,8},
        new uint8_t[]{12, 		5,0,4,5,11,0,5,10,11,11,3,0},
        new uint8_t[]{12, 		0,1,9,8,4,10,8,10,11,10,4,5},
        new uint8_t[]{15, 		10,11,4,10,4,5,11,3,4,9,4,1,3,1,4},
        new uint8_t[]{12, 		2,5,10,3,5,2,3,4,5,3,8,4},
        new uint8_t[]{9, 		5,10,2,5,2,4,4,2,0},
        new uint8_t[]{15, 		3,10,2,3,5,10,3,8,5,4,5,8,0,1,9},
        new uint8_t[]{12, 		5,10,2,5,2,4,1,9,2,9,4,2},
        new uint8_t[]{12, 		2,5,1,2,8,5,2,11,8,4,5,8},
        new uint8_t[]{15, 		0,4,11,0,11,3,4,5,11,2,11,1,5,1,11},
        new uint8_t[]{15, 		0,2,5,0,5,9,2,11,5,4,5,8,11,8,5},
        new uint8_t[]{6, 		9,4,5,2,11,3},
        new uint8_t[]{9, 		8,4,5,8,5,3,3,5,1},
        new uint8_t[]{6, 		0,4,5,1,0,5},
        new uint8_t[]{12, 		8,4,5,8,5,3,9,0,5,0,3,5},
        new uint8_t[]{3, 		9,4,5},
        new uint8_t[]{9, 		4,11,7,4,9,11,9,10,11},
        new uint8_t[]{12, 		0,8,3,4,9,7,9,11,7,9,10,11},
        new uint8_t[]{12, 		1,10,11,1,11,4,1,4,0,7,4,11},
        new uint8_t[]{15, 		3,1,4,3,4,8,1,10,4,7,4,11,10,11,4},
        new uint8_t[]{12, 		2,9,10,2,7,9,2,3,7,7,4,9},
        new uint8_t[]{15, 		9,10,7,9,7,4,10,2,7,8,7,0,2,0,7},
        new uint8_t[]{15, 		3,7,10,3,10,2,7,4,10,1,10,0,4,0,10},
        new uint8_t[]{6, 		1,10,2,8,7,4},
        new uint8_t[]{12, 		4,11,7,9,11,4,9,2,11,9,1,2},
        new uint8_t[]{15, 		9,7,4,9,11,7,9,1,11,2,11,1,0,8,3},
        new uint8_t[]{9, 		11,7,4,11,4,2,2,4,0},
        new uint8_t[]{12, 		11,7,4,11,4,2,8,3,4,3,2,4},
        new uint8_t[]{9, 		4,9,1,4,1,7,7,1,3},
        new uint8_t[]{12, 		4,9,1,4,1,7,0,8,1,8,7,1},
        new uint8_t[]{6, 		4,0,3,7,4,3},
        new uint8_t[]{3, 		4,8,7},
        new uint8_t[]{6, 		9,10,8,10,11,8},
        new uint8_t[]{9, 		3,0,9,3,9,11,11,9,10},
        new uint8_t[]{9, 		0,1,10,0,10,8,8,10,11},
        new uint8_t[]{6, 		3,1,10,11,3,10},
        new uint8_t[]{9, 		2,3,8,2,8,10,10,8,9},
        new uint8_t[]{6, 		9,10,2,0,9,2},
        new uint8_t[]{12, 		2,3,8,2,8,10,0,1,8,1,10,8},
        new uint8_t[]{3, 		1,10,2},
        new uint8_t[]{9, 		1,2,11,1,11,9,9,11,8},
        new uint8_t[]{12, 		3,0,9,3,9,11,1,2,9,2,11,9},
        new uint8_t[]{6, 		0,2,11,8,0,11},
        new uint8_t[]{3, 		3,2,11},
        new uint8_t[]{6, 		1,3,8,9,1,8},
        new uint8_t[]{3, 		0,9,1},
        new uint8_t[]{3, 		0,3,8},
        new uint8_t[]{0}

};
uint16_t *ChunkRenderer::edgeTable = new uint16_t[] {
        0x0000, 0x0109, 0x0203, 0x030a, 0x080c, 0x0905, 0x0a0f, 0x0b06,
        0x0406, 0x050f, 0x0605, 0x070c, 0x0c0a, 0x0d03, 0x0e09, 0x0f00,
        0x0190, 0x0099, 0x0393, 0x029a, 0x099c, 0x0895, 0x0b9f, 0x0a96,
        0x0596, 0x049f, 0x0795, 0x069c, 0x0d9a, 0x0c93, 0x0f99, 0x0e90,
        0x0230, 0x0339, 0x0033, 0x013a, 0x0a3c, 0x0b35, 0x083f, 0x0936,
        0x0636, 0x073f, 0x0435, 0x053c, 0x0e3a, 0x0f33, 0x0c39, 0x0d30,
        0x03a0, 0x02a9, 0x01a3, 0x00aa, 0x0bac, 0x0aa5, 0x09af, 0x08a6,
        0x07a6, 0x06af, 0x05a5, 0x04ac, 0x0faa, 0x0ea3, 0x0da9, 0x0ca0,
        0x08c0, 0x09c9, 0x0ac3, 0x0bca, 0x00cc, 0x01c5, 0x02cf, 0x03c6,
        0x0cc6, 0x0dcf, 0x0ec5, 0x0fcc, 0x04ca, 0x05c3, 0x06c9, 0x07c0,
        0x0950, 0x0859, 0x0b53, 0x0a5a, 0x015c, 0x0055, 0x035f, 0x0256,
        0x0d56, 0x0c5f, 0x0f55, 0x0e5c, 0x055a, 0x0453, 0x0759, 0x0650,
        0x0af0, 0x0bf9, 0x08f3, 0x09fa, 0x02fc, 0x03f5, 0x00ff, 0x01f6,
        0x0ef6, 0x0fff, 0x0cf5, 0x0dfc, 0x06fa, 0x07f3, 0x04f9, 0x05f0,
        0x0b60, 0x0a69, 0x0963, 0x086a, 0x036c, 0x0265, 0x016f, 0x0066,
        0x0f66, 0x0e6f, 0x0d65, 0x0c6c, 0x076a, 0x0663, 0x0569, 0x0460,
        0x0460, 0x0569, 0x0663, 0x076a, 0x0c6c, 0x0d65, 0x0e6f, 0x0f66,
        0x0066, 0x016f, 0x0265, 0x036c, 0x086a, 0x0963, 0x0a69, 0x0b60,
        0x05f0, 0x04f9, 0x07f3, 0x06fa, 0x0dfc, 0x0cf5, 0x0fff, 0x0ef6,
        0x01f6, 0x00ff, 0x03f5, 0x02fc, 0x09fa, 0x08f3, 0x0bf9, 0x0af0,
        0x0650, 0x0759, 0x0453, 0x055a, 0x0e5c, 0x0f55, 0x0c5f, 0x0d56,
        0x0256, 0x035f, 0x0055, 0x015c, 0x0a5a, 0x0b53, 0x0859, 0x0950,
        0x07c0, 0x06c9, 0x05c3, 0x04ca, 0x0fcc, 0x0ec5, 0x0dcf, 0x0cc6,
        0x03c6, 0x02cf, 0x01c5, 0x00cc, 0x0bca, 0x0ac3, 0x09c9, 0x08c0,
        0x0ca0, 0x0da9, 0x0ea3, 0x0faa, 0x04ac, 0x05a5, 0x06af, 0x07a6,
        0x08a6, 0x09af, 0x0aa5, 0x0bac, 0x00aa, 0x01a3, 0x02a9, 0x03a0,
        0x0d30, 0x0c39, 0x0f33, 0x0e3a, 0x053c, 0x0435, 0x073f, 0x0636,
        0x0936, 0x083f, 0x0b35, 0x0a3c, 0x013a, 0x0033, 0x0339, 0x0230,
        0x0e90, 0x0f99, 0x0c93, 0x0d9a, 0x069c, 0x0795, 0x049f, 0x0596,
        0x0a96, 0x0b9f, 0x0895, 0x099c, 0x029a, 0x0393, 0x0099, 0x0190,
        0x0f00, 0x0e09, 0x0d03, 0x0c0a, 0x070c, 0x0605, 0x050f, 0x0406,
        0x0b06, 0x0a0f, 0x0905, 0x080c, 0x030a, 0x0203, 0x0109, 0x0000
};