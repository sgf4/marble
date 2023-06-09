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

#pragma once
#include "gl.hpp"
#include <vector>

namespace ME {

namespace GL {

class Model {
    struct Mesh {
		GL::VAO vao;
        GL::VBO vboVertices, vboNormals, vboIndices;
        u32 numIndices {};
		//std::vector<u32> vert_indices;
		u32 tex_handle;
	};

    enum {
        APOS,
        ANORMALS
    };

public:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

    Model(const unsigned char* fdata, u64 size);
    void draw();
};

};

};
