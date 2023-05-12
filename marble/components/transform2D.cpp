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

#include <marble/components/transform2d.hpp>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ME;

Transform2D::Transform2D(glm::vec2 position, glm::vec2 scale, float rotation)
: position(position),
  scale(scale),
  rotation(rotation) {

}

static float clampRotation(float& f) {
    f -= 360 * (int)(f/180);
    return f;
}

void Transform2D::update() {
    model = glm::mat3 { 1.f };
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(clampRotation(rotation)));
    model = glm::scale(model, scale);
}

void Transform2D::update(GL::Shader& s) {
    update();
    updateUniforms(s);
}

void Transform2D::updateUniforms(GL::Shader& s) {
    glUseProgram(s);
    glUniformMatrix3fv(s.getUniform("umodel"), 1, GL_FALSE, glm::value_ptr(model));
    glUseProgram(0);
}
