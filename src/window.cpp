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

#include <marble/window.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>

static const char* mainFboVert = R"(
#version 330 core
layout (location = 0) in vec2 apos;

out vec2 tex_coord;

void main() {
    tex_coord = apos+vec2(0.5, 0.5);
    gl_Position = vec4(apos * 2.0, 0.0, 1.0);
}
)";

static const char* mainFboFrag = R"(
#version 330 core

in vec2 tex_coord;
out vec4 out_color;

uniform sampler2D utex;

void main() {
    vec4 c = texture(utex, tex_coord);
    out_color = c;
}
)";

GLFWInstance::GLFWInstance(Window& w) {
    int result = glfwInit();
    assert(result);
    ((void) result);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw = glfwCreateWindow(w.getResolution().x, w.getResolution().y, w.getTitle(), NULL, NULL);
    assert(glfw);

    glfwMakeContextCurrent(glfw);

}

GLFWInstance::~GLFWInstance() {
    glfwDestroyWindow(glfw);
    glfwTerminate();
}

Window::Window() :
   resolution {640, 480},
   offset(),
   size(640, 480),
   title("omg"),
   vsync(false),
   fullscreen(false),
   cursorIshiden(false),
   fboShader(mainFboVert, mainFboFrag) {

    glfwSetWindowUserPointer(glfwInstance, (void*) this);

    glfwSetWindowSizeCallback(glfwInstance, [] (GLFWwindow* wnd, int w, int h) {
        static_cast<Window*>(glfwGetWindowUserPointer(wnd))->onResize(w, h);
    });

    glfwSetCursorPosCallback(glfwInstance, [] (GLFWwindow* wnd, double x, double y) {
        static_cast<Window*>(glfwGetWindowUserPointer(wnd))->onMouseEvent({x, y});
    });

    //window_hide_cursor();

    // Init window fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Default
    glfwSwapInterval(vsync);

    // Input
    glfwSetKeyCallback(glfwInstance, [] (GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == 2) return;
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onKeyEvent(key, action);
    });

    std::fill_n(keys, sizeof(keys), KEY_STATE_RELEASED_REPEAT);
    // glfwSetMouseButtonCallback(glfwInstance, mouse_button_callback);

    onResize(size.x, size.y);
}

void Window::updateUniforms(GL::Shader& s) {
    glUseProgram(s);
    glUniform2f(s.getUniform("uwsize"), resolution.x, resolution.y);
    glUseProgram(0);
}

void Window::onResize(int w, int h) {
    size.x = w;
    size.y = h;

    offset.x = 0;
    offset.y = 0;

    if ((float)w / h > (float)resolution.x/resolution.y) {
        size.x = h*resolution.x/resolution.y;
        size.y = h;
        offset.x = (w-size.x)/2;
    } else {
        size.x = w;
        size.y = w*resolution.y/resolution.x;
        offset.y = (h-size.y)/2;
    }
}


void Window::setFullscreen(bool v) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    static int lastpos[] = {0, 0};
    static int lastsize[] = {0, 0};
    if (v) {
        glfwGetWindowPos(glfwInstance, &lastpos[0], &lastpos[1]);
        glfwGetWindowSize(glfwInstance, &lastsize[0], &lastsize[1]);
        glfwSetWindowMonitor(glfwInstance, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(glfwInstance, NULL, lastpos[0], lastpos[1], lastsize[0], lastsize[1], mode->refreshRate);
    }
    fullscreen = v;
}

void Window::setVsync(bool v) {
    glfwSwapInterval(v);
    vsync = v;
}


void Window::update() {
    /* Render here */
    glDisable(GL_DEPTH_TEST);

    if (getKeyDown(KEY_F11)) {
        setFullscreen(!fullscreen);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, width, height);
    glViewport(offset.x, offset.y, size.x, size.y);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(glInstance.getSquareVAO());
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(fboShader);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(glfwInstance);

    /* Poll for and process events */
    glfwPollEvents();

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, resolution.x, resolution.y);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
}

void Window::hideCursor() {
    glfwSetInputMode(glfwInstance, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPos(window_glfw, window_width/2.f, window_height/2.f);
    cursorIshiden = true;
}

void Window::unhideCursor() {
    glfwSetInputMode(glfwInstance, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursorIshiden = false;
}

void Window::close() {
    glfwSetWindowShouldClose(glfwInstance, 1);
}

// Input
void Window::onKeyEvent(u32 key, bool pressed) {
    if (key >= sizeof(keys)) return;
    setKeyState(static_cast<Key>(key), pressed ? KEY_STATE_DOWN : KEY_STATE_RELEASED);
}

Window::KeyState Window::getKeyState(Key key) const {
    return keys[key];
}

void Window::setKeyState(Key key, KeyState state) {
    keys[key] = state;
}

bool Window::getKey(Key key) const {
    auto state = getKeyState(key);
    return state == KEY_STATE_DOWN || state == KEY_STATE_DOWN_REPEAT;
}

bool Window::getKeyDown(Key key) {
    auto state = getKeyState(key);
    bool pressed = state == KEY_STATE_DOWN;
    if (pressed) setKeyState(key, KEY_STATE_DOWN_REPEAT);
    return pressed;
}

bool Window::getKeyReleased(Key key) {
    auto state = getKeyState(key);
    bool released = state == KEY_STATE_RELEASED;
    if (released) setKeyState(key, KEY_STATE_RELEASED_REPEAT);
    return released;
}

void Window::onMouseEvent(glm::vec2 pos) {
    pos.y *= -1;
    mouseDirection = pos-mousePosition;
    mousePosition = pos;
}

glm::vec2 Window::getMouseDir() {
    auto dir = mouseDirection;
    mouseDirection = {0, 0};
    return dir;
}
