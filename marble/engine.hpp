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
#include "integers.hpp"
#include <functional>
#include <memory>

namespace ME {

class Window;
class World;

namespace GL {
    class Shader;
}

class Engine;
inline Engine* engine;

class Engine {
    struct AssignPtr { AssignPtr(Engine* e) { engine = e; } } assignPtr { this };
    class EngineImpl; std::unique_ptr<EngineImpl> pimpl;

public:
    Engine();
    ~Engine();

    void update();
    void close();

    World& addWorld(World* world);

    template<typename T, typename... Ts>
    World& addWorld(Ts&&... args) {
        return addWorld(new T(std::forward<Ts>(args)...));
    }

    void delWorld(World* world);

    Window& getWindow();
    GL::Shader& getShader(const char* name);
    void forEachShader(const std::function<void(GL::Shader&)>&);
    u64 getNewId();
};

}
