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
#include <glm/vec3.hpp>
#include "../component.hpp"

#define TRANSFORM ENTITY.getComponent<Transform>()

namespace ME {

class Transform : public Component {
public:
    glm::mat4 model { 1.f };
    glm::vec3 position {};
    glm::vec3 scale {1, 1, 1};
    glm::vec3 rotation {};
    bool modelNeedsUpdate { true };

    Transform& operator=(const Transform& other);

    void update();
    Transform& setPosition(glm::vec3 p) { position = p; modelNeedsUpdate = true; return *this; }
    Transform& setPosition(float x, float y, float z) { position = {x, y, z}; modelNeedsUpdate = true; return *this; }

    Transform& setScale(glm::vec3 s) { scale = s; modelNeedsUpdate = true; return *this; }
    Transform& setScale(float x, float y, float z) { scale = {x, y, z}; modelNeedsUpdate = true; return *this; }

    Transform& setRotation(glm::vec3 r) { rotation = r; modelNeedsUpdate = true; return *this; }
    Transform& setRotation(float x, float y, float z) { rotation = {x, y, z}; modelNeedsUpdate = true; return *this; }

    Transform& addRotation(glm::vec3 r) { rotation += r; modelNeedsUpdate = true; return *this; }
    Transform& addRotation(float x, float y, float z) { rotation += glm::vec3{x, y, z}; modelNeedsUpdate = true; return *this; }

    void submitUniforms();
};

}
