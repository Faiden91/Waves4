#include "BSPMap.h"
#include <iostream>
#include <vector>
#include <fstream>

BSPMap::BSPMap() {
    // Constructor initialization if needed
}

BSPMap::~BSPMap() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    // Cleanup resources if necessary
}

bool BSPMap::LoadAllLumps(const std::string& filename)
{
    if (!Load(filename)) {
        std::cerr << "Failed to load BSP file." << std::endl;
        return false;
    }

    if (!LoadEntities()) {
        std::cerr << "Failed to load entities from BSP file." << std::endl;
        return false;
    }

    if (!LoadTextures()) {
        std::cerr << "Failed to load textures from BSP file." << std::endl;
        return false;
    }

    if (!LoadPlanes()) {
        std::cerr << "Failed to load planes from BSP file." << std::endl;
        return false;
    }

    if (!LoadNodes()) {
        std::cerr << "Failed to load planes from BSP file." << std::endl;
        return false;
    }

    if (!LoadVertices()) {
        std::cerr << "Failed to load planes from BSP file." << std::endl;
        return false;
    }

    if (!LoadFaces()) {
        std::cerr << "Failed to load planes from BSP file." << std::endl;
        return false;
    }

    return true;
}

bool BSPMap::Load(const std::string& filename) {
    fileStream.open(filename, std::ios::binary);
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open the BSP file: " << filename << std::endl;
        return false;
    }

    // Here, you would normally read the header and lumps information
    // Example:
    fileStream.read(reinterpret_cast<char*>(&header), sizeof(BSPHeader));
    if (strncmp(header.magic, "IBSP", 4) != 0 || header.version != 0x2e) {
        std::cerr << "Invalid file format or version." << std::endl;
        return false;
    }
    fileStream.read(reinterpret_cast<char*>(&lumps), sizeof(lumps));

    return true;
}

bool BSPMap::LoadEntities() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading entities." << std::endl;
        return false;
    }

    auto& entitiesLump = lumps[static_cast<int>(LumpType::Entities)];
    if (entitiesLump.length <= 0) {
        std::cerr << "Entities lump is empty or not present." << std::endl;
        return false;
    }

    std::vector<char> lumpData(entitiesLump.length + 1); // +1 for null-termination
    fileStream.seekg(entitiesLump.offset);
    fileStream.read(lumpData.data(), entitiesLump.length);
    lumpData[entitiesLump.length] = '\0'; // Ensure null-termination

    entities.assign(lumpData.data(), entitiesLump.length);
    std::cout << "Entities Lump Data:\n" << entities << std::endl;

    return true;
}

bool BSPMap::LoadTextures() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading textures." << std::endl;
        return false;
    }

    auto& texturesLump = lumps[static_cast<int>(LumpType::Textures)];
    if (texturesLump.length <= 0) {
        std::cerr << "Textures lump is empty or not present." << std::endl;
        return false;
    }

    // Move to the start of the textures lump in the file
    fileStream.seekg(texturesLump.offset);

    char textureName[64]; // Assuming a fixed size for texture names
    int numberOfTextures = texturesLump.length / sizeof(textureName);
    for (int i = 0; i < numberOfTextures; ++i) {
        fileStream.read(textureName, sizeof(textureName));
        // Ensure the string is null-terminated
        textureName[sizeof(textureName) - 1] = '\0';

        // Add the texture information to the textures vector
        textures.emplace_back(TextureInfo{ std::string(textureName) });

        // Print the name of the texture
        std::cout << "Loaded Texture: " << textureName << std::endl;
    }

    // Textures have been loaded and names printed to console.
    return true;
}

bool BSPMap::LoadPlanes() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading planes." << std::endl;
        return false;
    }

    auto& planesLump = lumps[static_cast<int>(LumpType::Planes)];
    if (planesLump.length <= 0) {
        std::cerr << "Planes lump is empty or not present." << std::endl;
        return false;
    }

    int numPlanes = planesLump.length / sizeof(Plane);
    planes.resize(numPlanes); // Prepare the vector to hold all the planes

    // Move to the start of the planes lump in the file
    fileStream.seekg(planesLump.offset);

    // Read the planes data directly into the vector
    fileStream.read(reinterpret_cast<char*>(planes.data()), planesLump.length);

    // Optional: Print each plane's data for verification
    /*for (const auto& plane : planes) {
        std::cout << "Plane: Normal(" << plane.normal[0] << ", "
            << plane.normal[1] << ", " << plane.normal[2]
            << ") Distance: " << plane.distance << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadNodes() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading nodes." << std::endl;
        return false;
    }

    auto& nodesLump = lumps[static_cast<int>(LumpType::Nodes)];
    if (nodesLump.length <= 0) {
        std::cerr << "Nodes lump is empty or not present." << std::endl;
        return false;
    }

    int numNodes = nodesLump.length / sizeof(Node);
    nodes.resize(numNodes); // Prepare the vector to hold all the nodes

    // Move to the start of the nodes lump in the file
    fileStream.seekg(nodesLump.offset);

    // Read the nodes data directly into the vector
    fileStream.read(reinterpret_cast<char*>(nodes.data()), nodesLump.length);

    // Optional: Print each node's data for verification
    /*for (const auto& node : nodes) {
        std::cout << "Node: Plane " << node.plane
            << ", Children (" << node.children[0] << ", " << node.children[1] << ")"
            << ", Bounding Box ((" << node.mins[0] << ", " << node.mins[1] << ", " << node.mins[2] << "), "
            << "(" << node.maxs[0] << ", " << node.maxs[1] << ", " << node.maxs[2] << "))" << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadLeafs() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading leafs." << std::endl;
        return false;
    }

    auto& leafsLump = lumps[static_cast<int>(LumpType::Leafs)];
    if (leafsLump.length <= 0) {
        std::cerr << "Leafs lump is empty or not present." << std::endl;
        return false;
    }

    int numLeafs = leafsLump.length / sizeof(Leaf);
    leafs.resize(numLeafs); // Prepare the vector to hold all the leafs

    // Move to the start of the leafs lump in the file
    fileStream.seekg(leafsLump.offset);

    // Read the leafs data directly into the vector
    fileStream.read(reinterpret_cast<char*>(leafs.data()), leafsLump.length);

    // Optional: Print each leaf's data for verification
    /*for (const auto& leaf : leafs) {
        std::cout << "Leaf: Cluster " << leaf.cluster << ", Area " << leaf.area
            << ", BBox ((" << leaf.mins[0] << ", " << leaf.mins[1] << ", " << leaf.mins[2]
            << "), (" << leaf.maxs[0] << ", " << leaf.maxs[1] << ", " << leaf.maxs[2] << "))"
            << ", Faces (" << leaf.firstLeafFace << " to " << (leaf.firstLeafFace + leaf.numLeafFaces - 1) << ")"
            << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadLeafFaces() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading leaf faces." << std::endl;
        return false;
    }

    auto& leafFacesLump = lumps[static_cast<int>(LumpType::LeafFaces)];
    if (leafFacesLump.length <= 0) {
        std::cerr << "LeafFaces lump is empty or not present." << std::endl;
        return false;
    }

    int numLeafFaces = leafFacesLump.length / sizeof(int); // Assuming each leaf face index is an int
    leafFaces.resize(numLeafFaces); // Prepare the vector to hold all the leaf face indices

    // Move to the start of the leaf faces lump in the file
    fileStream.seekg(leafFacesLump.offset);

    // Read the leaf face indices directly into the vector
    fileStream.read(reinterpret_cast<char*>(leafFaces.data()), leafFacesLump.length);

    // Optional: Print each leaf face index for verification
    /*for (int i = 0; i < numLeafFaces; ++i) {
        std::cout << "LeafFace Index: " << leafFaces[i] << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadLeafBrushes() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading leaf brushes." << std::endl;
        return false;
    }

    auto& leafBrushesLump = lumps[static_cast<int>(LumpType::LeafBrushes)];
    if (leafBrushesLump.length <= 0) {
        std::cerr << "LeafBrushes lump is empty or not present." << std::endl;
        return false;
    }

    int numLeafBrushes = leafBrushesLump.length / sizeof(int); // Assuming each leaf brush index is an int
    leafBrushes.resize(numLeafBrushes); // Prepare the vector to hold all the leaf brush indices

    // Move to the start of the leaf brushes lump in the file
    fileStream.seekg(leafBrushesLump.offset);

    // Read the leaf brush indices directly into the vector
    fileStream.read(reinterpret_cast<char*>(leafBrushes.data()), leafBrushesLump.length);

    //// Optional: Print each leaf brush index for verification
    //for (int i = 0; i < numLeafBrushes; ++i) {
    //    std::cout << "LeafBrush Index: " << leafBrushes[i] << std::endl;
    //}

    return true;
}

bool BSPMap::LoadVertices() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading vertices." << std::endl;
        return false;
    }

    auto& verticesLump = lumps[static_cast<int>(LumpType::Vertices)];
    if (verticesLump.length <= 0) {
        std::cerr << "Vertices lump is empty or not present." << std::endl;
        return false;
    }

    int numVertices = verticesLump.length / sizeof(Vertex);
    vertices.resize(numVertices); // Prepare the vector to hold all the vertices

    // Move to the start of the vertices lump in the file
    fileStream.seekg(verticesLump.offset);

    // Read the vertices data directly into the vector
    fileStream.read(reinterpret_cast<char*>(vertices.data()), verticesLump.length);

    // Print details of each vertex
    /*for (const auto& vertex : vertices) {
        std::cout << "Vertex: Position(" << vertex.position[0] << ", "
            << vertex.position[1] << ", " << vertex.position[2]
            << "), TexCoord(" << vertex.texCoord[0] << ", " << vertex.texCoord[1]
            << "), LightMapCoord(" << vertex.lmCoord[0] << ", " << vertex.lmCoord[1]
            << "), Normal(" << vertex.normal[0] << ", " << vertex.normal[1] << ", " << vertex.normal[2]
            << "), Color(" << static_cast<int>(vertex.color[0]) << ", "
            << static_cast<int>(vertex.color[1]) << ", "
            << static_cast<int>(vertex.color[2]) << ", "
            << static_cast<int>(vertex.color[3]) << ")" << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadFaces() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading faces." << std::endl;
        return false;
    }

    auto& facesLump = lumps[static_cast<int>(LumpType::Faces)];
    if (facesLump.length <= 0) {
        std::cerr << "Faces lump is empty or not present." << std::endl;
        return false;
    }

    int numFaces = facesLump.length / sizeof(Face);
    faces.resize(numFaces); // Prepare the vector to hold all the faces

    // Move to the start of the faces lump in the file
    fileStream.seekg(facesLump.offset);

    // Read the faces data directly into the vector
    fileStream.read(reinterpret_cast<char*>(faces.data()), facesLump.length);

    // Optional: Print details of each face for verification
    for (const auto& face : faces) {
        std::cout << "Face: Type " << face.type << ", Texture Index " << face.texture
            << ", Num Vertices " << face.numVertices << std::endl;

        for (int i = 0; i < face.numVertices; ++i) {
            const Vertex& vertex = vertices[face.vertex + i];
            std::cout << "\tVertex " << i << ": Position(" << vertex.position[0] << ", "
                << vertex.position[1] << ", " << vertex.position[2] << ")" << std::endl;
        }
    }
    return true;
}

bool BSPMap::LoadBrushes() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading brushes." << std::endl;
        return false;
    }

    auto& brushesLump = lumps[static_cast<int>(LumpType::Brushes)];
    if (brushesLump.length <= 0) {
        std::cerr << "Brushes lump is empty or not present." << std::endl;
        return false;
    }

    int numBrushes = brushesLump.length / sizeof(Brush); // Assuming each brush is represented by a Brush struct
    brushes.resize(numBrushes); // Prepare the vector to hold all the brushes

    // Move to the start of the brushes lump in the file
    fileStream.seekg(brushesLump.offset);

    // Read the brushes data directly into the vector
    fileStream.read(reinterpret_cast<char*>(brushes.data()), brushesLump.length);

    // Optional: Print each brush's data for verification
    /*for (const auto& brush : brushes) {
        std::cout << "Brush: BrushSide " << brush.brushSide
            << ", NumSides " << brush.numSides
            << ", Texture " << brush.texture << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadBrushSides() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading brush sides." << std::endl;
        return false;
    }

    auto& brushSidesLump = lumps[static_cast<int>(LumpType::BrushSides)];
    if (brushSidesLump.length <= 0) {
        std::cerr << "BrushSides lump is empty or not present." << std::endl;
        return false;
    }

    int numBrushSides = brushSidesLump.length / sizeof(BrushSide); // Assuming each brush side is represented by a BrushSide struct
    brushSides.resize(numBrushSides); // Prepare the vector to hold all the brush sides

    // Move to the start of the brush sides lump in the file
    fileStream.seekg(brushSidesLump.offset);

    // Read the brush sides data directly into the vector
    fileStream.read(reinterpret_cast<char*>(brushSides.data()), brushSidesLump.length);

    // Optional: Print each brush side's data for verification
    /*for (const auto& brushSide : brushSides) {
        std::cout << "BrushSide: Plane " << brushSide.plane
            << ", Texture " << brushSide.texture << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadMeshVerts() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading mesh verts." << std::endl;
        return false;
    }

    auto& meshVertsLump = lumps[static_cast<int>(LumpType::MeshVerts)];
    if (meshVertsLump.length <= 0) {
        std::cerr << "MeshVerts lump is empty or not present." << std::endl;
        return false;
    }

    int numMeshVerts = meshVertsLump.length / sizeof(int); // Assuming each MeshVert index is an int
    meshVerts.resize(numMeshVerts); // Prepare the vector to hold all the mesh vertex indices

    // Move to the start of the MeshVerts lump in the file
    fileStream.seekg(meshVertsLump.offset);

    // Read the MeshVerts indices directly into the vector
    fileStream.read(reinterpret_cast<char*>(meshVerts.data()), meshVertsLump.length);

    // Optional: Print each MeshVert index for verification
    /*for (int i = 0; i < numMeshVerts; ++i) {
        std::cout << "MeshVert Index: " << meshVerts[i] << std::endl;
    }*/

    return true;
}

bool BSPMap::LoadLightmaps() {
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open for reading lightmaps." << std::endl;
        return false;
    }

    auto& lightmapsLump = lumps[static_cast<int>(LumpType::Lightmaps)];
    if (lightmapsLump.length <= 0) {
        std::cerr << "Lightmaps lump is empty or not present." << std::endl;
        return false;
    }

    // Example: Assuming each lightmap is of a fixed size for simplicity
    // In practice, you'll need to adjust based on your lightmap data's specifics
    const int lightmapSize = 128 * 128; // Common lightmap size in bytes for grayscale; adjust for RGB(A)
    int numLightmaps = lightmapsLump.length / lightmapSize;
    // Prepare storage for lightmap data; actual implementation depends on your rendering system

    // Move to the start of the lightmaps lump in the file
    fileStream.seekg(lightmapsLump.offset);

    // Example: Read lightmap data
    // This is highly dependent on how you plan to use the lightmaps in your rendering system
    for (int i = 0; i < numLightmaps; ++i) {
        std::vector<unsigned char> lightmapData(lightmapSize);
        fileStream.read(reinterpret_cast<char*>(lightmapData.data()), lightmapSize);
        // Process or store the lightmap data as needed for your application
    }

    return true;
}

const std::vector<Face>& BSPMap::GetFaces() const {
    return faces;
}

const std::vector<Vertex>& BSPMap::GetVertex() const {
    return vertices;
}

