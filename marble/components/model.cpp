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

#include "model.hpp"
#include "transform.hpp"
#include "shader.hpp"

using namespace ME;

static const char* modelShaderVert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 normal;
out vec3 pos;

void main() {
    pos = vec3(uModel * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(uModel))) * aNormal;
    gl_Position = uProj * uView * vec4(pos, 1.0);
}
)";

static const char* modelShaderFrag = R"(
#version 330 core

in vec3 pos;
in vec3 normal;
out vec4 outColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(uLightPos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 result = (ambient + diffuse + specular) * vec3(1.0, 0.0, 0.0);

    outColor = vec4(result, 1.0);
}
)";

void Model::init() {
    addComponent<Transform>();

    if (!hasComponent<Shader>()) {
        addComponent<Shader>().setShader(CM.getSystem<Model>().shader);
    }
}

void Model::update() {
    if (!model) return;

    Shader& s = getComponent<Shader>();
    Transform& t = getComponent<Transform>();
    s->use();
    t.submitUniforms();
    model->draw();
    s->unuse();
}

ComponentSystem<Model>::ComponentSystem() {
    shader.compile(modelShaderVert, modelShaderFrag);
}
