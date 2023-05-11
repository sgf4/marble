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
#include "gl/gl.hpp"
#include "keycodes.hpp"
#include "integers.hpp"
#include "engine.hpp"
#include <glm/vec2.hpp>

#define WINDOW engine->getWindow()
#define WINDOW_RX WINDOW.getResolution().x
#define WINDOW_RY WINDOW.getResolution().y

struct GLFWwindow;
extern "C" int glfwWindowShouldClose(GLFWwindow*);

class Window;

struct GLFWInstance {
    GLFWwindow* glfw;

    operator GLFWwindow*() {
        return glfw;
    }

    GLFWInstance(Window& w);
    ~GLFWInstance();
};

class Window {
    glm::uvec2 resolution, offset, size;

    const char* title;
    bool vsync, fullscreen, cursorIshiden;

    glm::vec2 mouseDirection {}, mousePosition {};

    GLFWInstance glfwInstance {*this};
    GL::Instance glInstance;
    GL::Shader fboShader;
    GL::FBO fbo;
    GL::RBO depthRBO;
    GL::Texture texture;

    enum KeyState : u8 {
        KEY_STATE_RELEASED,
        KEY_STATE_RELEASED_REPEAT,
        KEY_STATE_DOWN,
        KEY_STATE_DOWN_REPEAT,
    };

    KeyState keys[512];

    void onKeyEvent(u32 key, bool pressed);
    void onMouseEvent(glm::vec2 pos);

public:
    Window();
    void update();
    void close();
    void hideCursor();
    void unhideCursor();
    void setFullscreen(bool);
    void setVsync(bool);
    void onResize(int w, int h);

    void updateUniforms(GL::Shader& s);
    bool shouldClose() { return glfwWindowShouldClose(glfwInstance); }
    GLFWInstance& getGlfwInstance() { return glfwInstance; }
    auto getSize() const { return size; }
    auto getResolution() const { return resolution; }
    auto getTitle() const { return title; }

    float getResolutionRatio() {
        return (float)resolution.x/resolution.y;
    }

    KeyState getKeyState(Key key) const;
    void setKeyState(Key key, KeyState value);
    bool getKey(Key key) const;
    bool getKeyDown(Key key);
    bool getKeyReleased(Key key);
    glm::vec2 getMouseDir();
};
