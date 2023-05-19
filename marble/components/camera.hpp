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
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../component.hpp"
#include "transform.hpp"

namespace ME {

class Camera : public Component {
    void updateControl();
public:
    bool control;
    glm::vec3 direction;
    glm::vec3 up;
    glm::mat4 proj;
    float fov, yaw, pitch, sensibility;

    Camera();

    void init();
    void update();
    //void updateUniforms(GL::Shader&);
    Camera& setControl(bool v) { control = v; return *this; }
};

}
