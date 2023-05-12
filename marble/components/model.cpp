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

#include <marble/components/transform.hpp>
#include <marble/components/model.hpp>

using namespace ME;

static const char* modelShaderVert = R"(
#version 330 core
layout (location = 0) in vec2 apos;

uniform mat4 umodel;
uniform mat4 uview;
uniform mat4 uproj;

out vec3 color;

void main() {
    gl_Position = uproj * uview * umodel * vec4(apos, 0.0, 1.0);
}
)";

static const char* modelShaderFrag = R"(
#version 330 core

out vec4 out_color;

void main() {
    out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
void Model::init() {
    addComponent<Transform>();
}

ComponentSystem<Model>::ComponentSystem() {
    shader.compile(modelShaderVert, modelShaderFrag);
}
