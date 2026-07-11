//
// Created by seongcheoljeon on 2026-07-11.
//

#include "model.h"

#include "../cmake-build-debug/_deps/assimp-src/contrib/openddlparser/include/openddlparser/OpenDDLCommon.h"

ModelUPtr Model::Load( const std::string& filename )
{
    auto model = ModelUPtr(new Model());
    if (!model->_LoadByAssimp(filename))
    {
        return nullptr;
    }
    return model;
}

void Model::Draw(const Program* program) const
{
    for (auto& mesh : _meshes)
    {
        mesh->Draw(program);
    }
}

bool Model::_LoadByAssimp( const std::string& filename )
{
    Assimp::Importer importer;
    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SPDLOG_ERROR("failed to load model: {}", filename);
        return false;
    }

    auto dirname      = filename.substr(0, filename.find_last_of("/"));
    auto _LoadTexture = [&] ( aiMaterial* material, aiTextureType type ) -> TextureSPtr {
        if (material->GetTextureCount(type) <= 0)
        {
            return nullptr;
        }
        aiString filepath;
        material->GetTexture(type, 0, &filepath);
        auto image = Image::Load(fmt::format("{}/{}", dirname, filepath.C_Str()));
        if (!image)
        {
            return nullptr;
        }
        return Texture::CreateFromImage(image.get());
    };

    for (uint32_t i=0; i<scene->mNumMaterials; ++i)
    {
        auto material = scene->mMaterials[i];
        auto gl_material = Material::Create();
        gl_material->_diffuse = _LoadTexture(material, aiTextureType_DIFFUSE);
        gl_material->_specular = _LoadTexture(material, aiTextureType_SPECULAR);
        _materials.push_back(std::move(gl_material));
    }

    _ProcessNode(scene->mRootNode, scene);
    return true;
}

void Model::_ProcessMesh( aiMesh* mesh, const aiScene* scene )
{
    SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}",
                mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices);

    const bool has_texcoord = mesh->mTextureCoords[0] != nullptr;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex& v  = vertices[i];
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal   = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        v.texcoord = has_texcoord
                         ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                         : glm::vec2(0.0f);
    }

    std::vector<uint32_t> indices;
    indices.resize(mesh->mNumFaces * 3);

    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        indices[i * 3]     = mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
    }

    auto gl_mesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (mesh->mMaterialIndex >= 0)
    {
        gl_mesh->SetMaterial(_materials[mesh->mMaterialIndex]);
    }
    _meshes.push_back(std::move(gl_mesh));
}

void Model::_ProcessNode( aiNode* node, const aiScene* scene )
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        auto mesh_index = node->mMeshes[i];
        auto mesh       = scene->mMeshes[mesh_index];
        _ProcessMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        _ProcessNode(node->mChildren[i], scene);
    }
}