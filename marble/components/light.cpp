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

#include "light.hpp"
#include "transform.hpp"
#include "../gl/gl.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace ME;

void Light::init() {
    addComponent<Transform>().setPosition(10.0, 0.0, -10.0);
}

void Light::update() {
    Transform& t = getComponent<Transform>();
    // Update uniforms
    for (GL::Shader* s : GL::Shader::shaders) {
        glUseProgram(*s);
        try { glUniform3fv(s->getUniform("uLightPos"), 1, glm::value_ptr(t.position)); } catch(...) {}
        try { glUniform3fv(s->getUniform("uLightColor"), 1, glm::value_ptr(color)); } catch(...) {}
    }
}
