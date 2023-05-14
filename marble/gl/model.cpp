/*  Copyright 2023 sgf4

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <marble/gl/model.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <assimp/Importer.hpp>

using namespace ME::GL;

Model::Model(const unsigned char* fdata, u64 size) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(fdata, size, aiProcessPreset_TargetRealtime_Quality | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cerr << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
        return;
    }

    meshes.resize(scene->mNumMeshes);

    for (u32 i=0; i<meshes.size(); i++) {
        const aiMesh* paiMesh = scene->mMeshes[i];
        Mesh& m = meshes[i];

        std::vector<u32> indices;
        indices.reserve(paiMesh->mNumFaces*3);

        for (u32 j=0; j<paiMesh->mNumFaces; j++) {
            const auto& face = paiMesh->mFaces[j];
            assert(face.mNumIndices == 3);
            for (int k=0; k<face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k]);
            }
        }
        m.numIndices = indices.size();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vboIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(u32), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(m.vao);

        if (paiMesh->HasPositions()) {
            glBindBuffer(GL_ARRAY_BUFFER, m.vboVertices);
            glBufferData(GL_ARRAY_BUFFER, paiMesh->mNumVertices*sizeof(aiVector3D), paiMesh->mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(APOS);
            glVertexAttribPointer(APOS, 3, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), (void*) 0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        if (paiMesh->HasNormals()) {
            glBindBuffer(GL_ARRAY_BUFFER, m.vboNormals);
            glBufferData(GL_ARRAY_BUFFER, paiMesh->mNumVertices*sizeof(aiVector3D), paiMesh->mNormals, GL_STATIC_DRAW);

            glEnableVertexAttribArray(ANORMALS);
            glVertexAttribPointer(ANORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), (void*) 0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glBindVertexArray(0);
    }
}

void Model::draw() {
    for (Mesh& m : meshes) {
        glBindVertexArray(m.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vboIndices);
        glDrawElements(GL_TRIANGLES, m.numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

