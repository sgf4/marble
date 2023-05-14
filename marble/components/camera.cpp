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

#include "camera.hpp"
#include "transform.hpp"
#include "../entity.hpp"
#include "../window.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdexcept>

using namespace ME;

void Camera::init() {
    getEntity().addComponent<Transform>();
}

void Camera::update() {
    if (control) updateControl();

    auto& t = getComponent<Transform>();
    pitch = glm::clamp(pitch, -89.f, 89.f);
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction = glm::normalize(direction);
    t.model = glm::lookAt(t.position, t.position+direction, up);

    // set projection
    proj = glm::perspective(glm::radians(fov), (float)WINDOW.getResolutionRatio(), 0.01f, 10000.0f);
    yaw = std::fmod(yaw, 360.f);

    // Update uniforms
    for (GL::Shader* s : GL::Shader::shaders) {
        glUseProgram(*s);
        try { glUniformMatrix4fv(s->getUniform("uView"), 1, GL_FALSE, glm::value_ptr(t.model)); } catch(...) {}
        try { glUniformMatrix4fv(s->getUniform("uProj"), 1, GL_FALSE, glm::value_ptr(proj)); } catch(...) {}
        try { glUniform3fv(s->getUniform("uViewPos"), 1, glm::value_ptr(t.position)); } catch(...) {}
    }
    glUseProgram(0);
}

void Camera::updateControl() {
    auto dir = WINDOW.getMouseDir() * glm::vec2(sensibility);

    yaw += dir.x;
    pitch += dir.y;

    auto& t = getComponent<Transform>();
    glm::vec3 mov = {
        std::cos(glm::radians(yaw))*WTIME.delta*2.0,
        0.0,
        std::sin(glm::radians(yaw))*WTIME.delta*2.0
    };

    if (WINDOW.getKey(KEY_W)) t.position += mov;
    if (WINDOW.getKey(KEY_S)) t.position -= mov;
    if (WINDOW.getKey(KEY_D)) t.position += glm::cross(mov, up);
    if (WINDOW.getKey(KEY_A)) t.position -= glm::cross(mov, up);


}
