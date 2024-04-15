#ifndef BSPMAP_H
#define BSPMAP_H

#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h> // Make sure you have GLEW or an equivalent loader for OpenGL functions
#include "Renderer.h"
#include "Shader.h"

/*
Class to parse .bsp files. The files are organized by what we call "Lumps" so we will parse through each lump and retrieve the data we need.
*/

// Lump types
enum class LumpType {
    Entities = 0,
    Textures = 1,
    Planes = 2,
    Nodes = 3,
    Leafs = 4,
    LeafFaces = 5,
    LeafBrushes = 6,
    Models = 7,
    Brushes = 8,
    BrushSides = 9,
    Vertices = 10,
    MeshVerts = 11,
    Effects = 12,
    Faces = 13,
    Lightmaps = 14,
    LightVolumes = 15,
    VisData = 16,
    Count = 17 // Not a lump, but counts the number of lump types
};

struct BSPHeader {
    char magic[4]; // Should be "IBSP"
    int version; // Should be 0x2e for Quake III
};

struct BSPLump {
    int offset;
    int length;
};

//Struct for the textures
struct TextureInfo {
    std::string name; // Texture name
    // You can add other texture-related properties here, such as:
    // int flags;
    // int contents;
};

struct Plane {
    float normal[3]; // Normal vector to the plane (a, b, c)
    float distance; // Distance from the origin to the plane along its normal vector
};

struct Node {
    int plane;          // Index of the splitting plane
    int children[2];    // Indices of the child nodes (front, back)
    int mins[3];        // Bounding box min coordinate
    int maxs[3];        // Bounding box max coordinate
    // Add additional fields if necessary depending on the BSP format
};

struct Leaf {
    int cluster;       // Visibility data cluster index
    int area;          // Area this leaf is part of
    int mins[3];       // Minimum coordinates of the leaf's bounding box
    int maxs[3];       // Maximum coordinates of the leaf's bounding box
    int firstLeafFace; // Index of the first face in this leaf
    int numLeafFaces;  // Number of faces in this leaf
    // Add additional fields as necessary based on your BSP format
};

struct Brush {
    int brushSide;  // Index of the first brush side
    int numSides;   // Number of brush sides
    int texture;    // Texture index (not always used for collision-only brushes)
};

struct BrushSide {
    int plane;  // Index of the plane used by this brush side
    int texture; // Texture index used by this brush side (may be used for rendering or collision properties)
};

struct Vertex {
    float position[3]; // x, y, z coordinates
    float texCoord[2]; // Texture coordinates (s, t)
    float lmCoord[2];  // Lightmap coordinates
    float normal[3];   // Normal vector
    unsigned char color[4]; // RGBA color
};

struct Face {
    int texture; // Index of the texture
    int effect; // Index of the effect
    int type; // Type of face (polygon, patch, mesh, billboard)
    int vertex; // Index of the first vertex
    int numVertices; // Number of vertices
    int meshVertex; // Index of the first meshvertex
    int numMeshVertices; // Number of mesh vertices
    int lm_index; // Lightmap index
    int lm_start[2]; // Starting position of the lightmap
    int lm_size[2]; // Size of the lightmap
    float lm_origin[3]; // World space origin of lightmap
    float lm_vecs[2][3]; // World space vectors s and t for lightmap
    float normal[3]; // Surface normal
    int size[2]; // Patch dimensions
};



class BSPMap {
public:
    BSPMap();
    ~BSPMap();

    bool Load(const std::string& filename);

    //Functions for parsing each Lump
    bool LoadEntities();
    bool LoadTextures();
    bool LoadPlanes();
    bool LoadNodes();
    bool LoadLeafs();
    bool LoadLeafFaces();
    bool LoadLeafBrushes();
    bool LoadBrushes();
    bool LoadBrushSides();
    //Models here
    bool LoadVertices();
    bool LoadMeshVerts();
    bool LoadFaces();
    bool LoadLightmaps();

    bool LoadAllLumps(const std::string& filename);

    const std::vector<Face>& GetFaces() const;
    const std::vector<Vertex>& GetVertex() const;


private:
    std::ifstream fileStream;
    std::vector<char> fileData;

    BSPHeader header;
    BSPLump lumps[static_cast<int>(LumpType::Count)];

    std::string entities; // Store entity data. For now we are storing it in one large string.
    std::vector<TextureInfo> textures; // Vector to store loaded texture information
    // Other members for handling lumps, etc.
    std::vector<Plane> planes;
    std::vector<Node> nodes;
    std::vector<Leaf> leafs;
    std::vector<int> leafFaces;
    std::vector<int> leafBrushes;
    //Models here
    std::vector<Brush> brushes;
    std::vector<BrushSide> brushSides;
    std::vector<Vertex> vertices;
    std::vector<int> meshVerts;
    std::vector<Face> faces; // Vector to store loaded face information



    //std::vector<Vertex> myVertices;


    //std::vector<Face> faces; // Vector to store loaded faces
};

#endif // BSPMAP_H













