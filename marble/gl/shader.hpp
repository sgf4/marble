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
#include "../integers.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace ME {

namespace GL {

class Shader {
    u32 program {};
    std::unordered_map<std::string, u32> attributes;
    std::unordered_map<std::string, u32> uniforms;

public:
    static std::unordered_set<GL::Shader*> shaders;

    Shader();
    Shader(const char* vsource, const char* fsource);
    ~Shader();

    void compile(const char* vsource, const char* fsource);

    u32 getAttrib(const char* name) {
        return attributes.at(name);
    }
    u32 getUniform(const char* name) {
        return uniforms.at(name);
    }

    void use() {
        glUseProgram(program);
    }

    void unuse() {
        glUseProgram(0);
    }

    void use(std::function<void(void)> fn) {
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);

        glUseProgram(program);
        fn();
        glUseProgram(id);
    }

    Shader(Shader&& other) {
        *this = std::move(other);
    }

    Shader& operator=(Shader&& other);

    operator u32() {
        return program;
    }
};

}

}
