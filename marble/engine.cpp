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

#include "engine.hpp"
#include "window.hpp"
#include "world.hpp"

#include <memory>
#include <unordered_set>

using namespace ME;

class Engine::EngineImpl {
    friend Engine;
    Window window;
    Time time;
    std::unordered_set<World*> worlds;

public:
    EngineImpl() {
    }

    void update() {
        while (!window.shouldClose()) {
            try {
                window.update();
                time.update();

                // Update worlds
                for (World* world : worlds) {
                    world->updateWorld();
                    world->update();
                }
            } catch(std::string str) {
                std::cerr << "ERROR: " << str << std::endl;
            }
        }
    }

    void addWorld(World* world) {
        worlds.insert(world);
        World* w = World::current;
        World::current = world;
        world->init();
        World::current = w;
    }

    void delWorld(World* world) {
        worlds.erase(world);
        delete world;
    }

    void close() {
        window.shouldClose();
    }

    u64 getNewId() {
        static u64 id {};
        return id++;
    }

    ~EngineImpl() {
        for (World* world : worlds) {
            delete world;
        }
    }
};

Engine::Engine() : pimpl(std::make_unique<EngineImpl>()) {
}

void Engine::update() {
    pimpl->update();
}

void Engine::close() {
    pimpl->close();
}

u64 Engine::getNewId() {
    return pimpl->getNewId();
}

Window& Engine::getWindow() {
    return pimpl->window;
}

World& Engine::addWorld(World* world) {
    pimpl->addWorld(world);
    return *world;
}

void Engine::delWorld(World* world) {
    pimpl->delWorld(world);
}

Engine::~Engine() {
}
