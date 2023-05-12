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
#include "time.hpp"
#include <memory>
#include <vector>
#include <unordered_set>
#include <marble/integers.hpp>

#define WORLD (*World::current)
#define WCAMERA (WORLD.getCamera())
#define WCAMERAC (WORLD.getCamera().getComponent<Camera>())
#define WTIME WORLD.time
#define CM WORLD.getComponentManager()

namespace ME {

class Entity;
class ComponentManager;

class World {
    std::unique_ptr<ComponentManager> componentManager;
    std::vector<Entity> entities;
    std::shared_ptr<u32> camera;
public:

    u64 id;
    Time time;

    inline static World* current;

    World();
    virtual ~World();

    Entity& addEntity();
    u32 getEntityCount();
    Entity& getEntity(u32 id);
    void delEntity(Entity& e);
    void delEntity(u32 id);
    Entity& getCamera() { return getEntity(*camera); }

    virtual void init() {}
    virtual void update() {}

    void updateWorld();
    void updateUniforms(GL::Shader& s);
    ComponentManager& getComponentManager() { return *componentManager; }
};

};
