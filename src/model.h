//
// Created by seongcheoljeon on 2026-07-11.
//

#ifndef OPENGL_PROJECT_MODEL_H
#define OPENGL_PROJECT_MODEL_H

#include "common.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model)

class Model
{
public:
    static ModelUPtr Load( const std::string& filename );

    [[nodiscard]] int GetMeshCount() const
    {
        return static_cast<int>(_meshes.size());
    }

    [[nodiscard]] MeshSPtr GetMesh( const int index ) const
    {
        return _meshes[index];
    }

    void Draw(const Program* program) const;

private:
    Model() = default;
    [[nodiscard]] bool _LoadByAssimp(const std::string& filename);
    void _ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void _ProcessNode(aiNode* node, const aiScene* scene);

private:
    std::vector<MeshSPtr> _meshes;
    std::vector<MaterialSPtr> _materials;
};

#endif //OPENGL_PROJECT_MODEL_H