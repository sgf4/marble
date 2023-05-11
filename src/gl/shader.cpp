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

#include <marble/gl/shader.hpp>
#include <marble/gl/gl.hpp>

using namespace ME::GL;

std::unordered_set<Shader*> Shader::shaders;

Shader::Shader() {}
Shader::Shader(const char* vsource, const char* fsource) {
    program = glCreateProgram();
    u32 vs = glCreateShader(GL_VERTEX_SHADER);
    u32 fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsource, 0);
    glShaderSource(fs, 1, &fsource, 0);
    glCompileShader(vs);
    glCompileShader(fs);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vs, 512, 0, infoLog);
        fprintf(stderr, "Error compiling vertex shader: %s\n", infoLog);
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fs, 512, 0, infoLog);
        fprintf(stderr, "Error compiling fragment shader %s\n", infoLog);
    }

    // Link shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Get attribs

    int nAttribs;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    for (int i=0; i<nAttribs; i++) {
        GLchar name[32];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(program, i, sizeof(name), &length, &size, &type, name);
        attributes[name] = glGetAttribLocation(program, name);
    }

    int nUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &nUniforms);
    for (int i = 0; i < nUniforms; i++) {
        GLchar name[32];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);
        uniforms[name] = glGetUniformLocation(program, name);
    }

    shaders.insert(this);
}

Shader& Shader::operator=(Shader&& other) {
    glDeleteProgram(program);
    program = other.program;
    other.program = 0;
    attributes = std::move(other.attributes);
    uniforms = std::move(other.uniforms);
    shaders.insert(this);

    return *this;
}

Shader::~Shader() {
    if (program) {
        shaders.erase(this);
        glDeleteProgram(program);
    }
}
