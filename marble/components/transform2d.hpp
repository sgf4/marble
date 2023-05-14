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
#include "../component.hpp"
#include "../gl/gl.hpp"
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

namespace ME {

class Transform2D : public Component {
public:
    Transform2D(glm::vec2 position = {0, 0}, glm::vec2 scale = {1, 1}, float rotation = 0);

    glm::mat3 model { 1.f };
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    void update();
    void update(GL::Shader& s);
    void updateUniforms(GL::Shader& s);
};

}
