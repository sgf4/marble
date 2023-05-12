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
#include "../integers.hpp"
#include "glad.hpp"

// void gl_task_queue_runtasks(void);
// void gl_task_queue_execute(void (*fn) (void*), void* arg);

namespace ME {

namespace GL {

struct ID {
    u32 id;
    ID(u32 id = 0) : id(id) {}

    operator u32() const {
        return id;
    }

    auto operator&() {
        return &id;
    }
};

struct VAO : ID {
    using ID::ID;

    VAO() {
        glGenVertexArrays(1, &id);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &id);
    }
};

struct VBO : ID {
    using ID::ID;

    VBO() {
        glGenBuffers(1, &id);
    }

    ~VBO() {
        glDeleteBuffers(1, &id);
    }
};

struct FBO : ID {
    using ID::ID;

    FBO() {
        glGenFramebuffers(1, &id);
    }

    ~FBO() {
        glDeleteFramebuffers(1, &id);
    }
};

struct RBO : ID {
    using ID::ID;

    RBO() {
        glGenRenderbuffers(1, &id);
    }

    ~RBO() {
        glDeleteRenderbuffers(1, &id);
    }
};

class Instance {
    struct I {
        I() {
            gladLoaderLoadGL();
        }
        ~I() {
            gladLoaderUnloadGL();
        }
    } loader;
    VAO squareVAO;
    VBO squareVBO;

public:

    u32 getSquareVAO() const {
        return squareVAO;
    }

    Instance();
    ~Instance();
};

inline Instance* instance;

}

}

#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"
