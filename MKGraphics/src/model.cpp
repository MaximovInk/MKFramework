
#include "model.h"

#include <algorithm>
#include <string>
#include "stb/stb_image.h"
//#include "spdlog/spdlog.h"
#include "MKUtils.h"
using namespace std;

/*
void model::draw(shader& shader, camera& camera)
{

    drawM(shader, camera.Matrix);
}

void model::drawM(shader& shader, glm::mat4 camMatrix)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawM(shader, camMatrix);
    }
}

*/


void model::draw()
{

}

void model::loadModel(std::string path)
{
    std::replace(path.begin(), path.end(), '\\', '/');

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);
    //| aiProcess_FindInvalidData | aiProcess_GenUVCoords | aiProcess_TransformUVCoords | aiProcess_ImproveCacheLocality | aiProcess_GenNormals | aiProcess_FlipUVs

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG::error("Assimp error {}", import.GetErrorString());
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

MKEngine::mesh model::processMesh(aiMesh* aiMesh, const aiScene* scene)
{
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    //std::vector<texture> textures;

    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        vertex vertex;
        glm::vec3 vector;
        vector.x = aiMesh->mVertices[i].x;
        vector.y = aiMesh->mVertices[i].y;
        vector.z = aiMesh->mVertices[i].z;
        vertex.position = vector;

        if (aiMesh->mNormals != nullptr) {
            vector.x = aiMesh->mNormals[i].x;
            vector.y = aiMesh->mNormals[i].y;
            vector.z = aiMesh->mNormals[i].z;

        }
        else {
            vector.x = 0;
            vector.y = 0;
            vector.z = 0;
        }
        vertex.normal = vector;

        if (aiMesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = aiMesh->mTextureCoords[0][i].x;
            vec.y = aiMesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }
        else
            vertex.texUV = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
    {
        aiFace face = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

   /*
    if (aiMesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
        std::vector<texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
   */

    return MKEngine::mesh(vertices, indices);
}

//std::vector<texture> textures_loaded;

unsigned int TextureFromFile(const char* filename);

/*

std::vector<texture> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;

        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            auto texPath = directory + "/" + str.C_Str();
            std::string texType = "diffuse";
            GLenum texSlot = 0;

            switch (type)
            {
            case aiTextureType_NONE:
                break;
            case aiTextureType_DIFFUSE:
                texType = "diffuse";
                texSlot = 0;
                break;
            case aiTextureType_SPECULAR:
                texType = "specular";
                texSlot = 1;
                break;
            case aiTextureType_AMBIENT:
                break;
            case aiTextureType_EMISSIVE:
                break;
            case aiTextureType_HEIGHT:
                break;
            case aiTextureType_NORMALS:
                break;
            case aiTextureType_SHININESS:
                break;
            case aiTextureType_OPACITY:
                break;
            case aiTextureType_DISPLACEMENT:
                break;
            case aiTextureType_LIGHTMAP:
                break;
            case aiTextureType_REFLECTION:
                break;
            case aiTextureType_BASE_COLOR:
                break;
            case aiTextureType_NORMAL_CAMERA:
                break;
            case aiTextureType_EMISSION_COLOR:
                break;
            case aiTextureType_METALNESS:
                break;
            case aiTextureType_DIFFUSE_ROUGHNESS:
                break;
            case aiTextureType_AMBIENT_OCCLUSION:
                break;
            case aiTextureType_UNKNOWN:
                break;
            case _aiTextureType_Force32Bit:
                break;
            default:
                break;
            }

            unsigned int texID = TextureFromFile(texPath.c_str());

            texture tex(texID, "diffuse", texPath);
            tex.path = str.C_Str();

            textures.push_back(tex);
            textures_loaded.push_back(tex);
        }
    }
    return textures;
}


*/

unsigned int TextureFromFile(const char* filename)
{



    unsigned int textureID = -1;

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);

    spdlog::info(" {} {} {} {}", filename, width, height, nrComponents);
    if (data)
    {
        glGenTextures(1, &textureID);

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3) {
            format = GL_RGB;

        }
        else if (nrComponents == 4)
            format = GL_RGBA;


        spdlog::info(" {}", format);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        /*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

        stbi_image_free(data);
    }
    else
    {
        spdlog::error("Texture failed to load at path: {}", filename);
        stbi_image_free(data);
    }

    return textureID;
}

