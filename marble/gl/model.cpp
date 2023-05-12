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

static const char* modelShaderVert = R"(
#version 330 core
layout (location = 0) in vec2 apos;

uniform mat4 umodel;
uniform mat4 uview;
uniform mat4 uproj;

out vec3 color;

void main() {
    gl_Position = uproj * uview * umodel * vec4(apos, 0.0, 1.0);
}
)";

static const char* modelShaderFrag = R"(
#version 330 core

out vec4 out_color;

void main() {
    out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

Shader Model::shader;

Model::Model(const char* fdata, u32 size) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(fdata, size, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cerr << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
        return;
    }

    meshes.resize(scene->mNumMeshes);

    for (u32 i=0; i<meshes.size(); i++) {
        const aiMesh* paiMesh = scene->mMeshes[i];
        Mesh& m = meshes[i];

        std::vector<glm::vec3> vertices;
        vertices.resize(paiMesh->mNumFaces*3);

        for (u32 j=0; paiMesh->mNumFaces; j++) {
            const auto& face = paiMesh->mFaces[j];
            assert(face.mNumIndices == 3);
            for (int k=0; k<face.mNumIndices; k++) {
                aiVector3D pos = paiMesh->mVertices[face.mIndices[k]];
                vertices[j*3+k] = {pos.x, pos.y, pos.z};
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(m.vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Model::draw() {

}


void Model::compileShader() {
    shader.compile(modelShaderVert, modelShaderFrag);
}
