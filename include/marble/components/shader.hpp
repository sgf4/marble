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
#include <marble/component.hpp>
#include <marble/gl/gl.hpp>

class Shader : public Component {

public:
    GL::Shader* m_shader {};
public:
    void setShader(GL::Shader& s) { m_shader = &s; }
    GL::Shader& getShader() { return *m_shader; }

    void use() {
        glUseProgram(*m_shader);
    }

    auto getAttrib(const char* a) {
        return m_shader->getAttrib(a);
    }

    auto getUniform(const char* u) {
        return m_shader->getUniform(u);
    }

    void unuse() {
        glUseProgram(0);
    }
};
