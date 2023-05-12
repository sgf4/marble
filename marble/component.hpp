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
#include "world.hpp"

#include <unordered_map>
#include <vector>
#include <bitset>

namespace ME {

class Entity;
struct Component {
    u32 id;
    u32 eId;

    struct Key {
        u32 type;
        u32 id;
    };

    Entity& getEntity() {
        return WORLD.getEntity(eId);
    }

    operator Entity&() {
        return getEntity();
    }

    template<typename T>
    T& addComponent();

    template<typename T>
    T& getComponent();

    template<typename T>
    void delComponent();

    virtual void init() {}
    virtual void update() {}
    virtual void destroy() {}

};

class BasicComponentSystemBase {
public:
    virtual Component& add() = 0;
    virtual void del(u32 id) = 0;
    virtual Component& get(u32 id) = 0;
    virtual void update() = 0;
    virtual u32 count() = 0;
    virtual ~BasicComponentSystemBase() {}
};

template<typename C>
class BasicComponentSystem : public BasicComponentSystemBase {
    std::vector<C> components;
public:
    BasicComponentSystem() {
        components.reserve(32);
    }

    C& add() {
        C& c = components.emplace_back();
        c.id = components.size()-1;
        return c;
    }

    void del(u32 id) {
        C& component = components[id];
        component = std::move(*--components.end());
        components.pop_back();
    }

    u32 count() {
        return components.size();
    }

    Component& get(u32 id) {
        return components[id];
    }

    void update() {
        for (C& component : components) {
            component.update();
        }
    }

    ~BasicComponentSystem() {}
};

template<typename C>
class ComponentSystem : public BasicComponentSystem<C> {
};

class ComponentSystemManager;

class ComponentManager {
    std::unordered_map<u32, std::unique_ptr<BasicComponentSystemBase>> systems;
public:

    inline static u32 cId {};

    template<typename T>
    static u32 getTypeId() {
        static u32 id {static_cast<u32>(-1)};
        if (id == -1) id = cId++;
        return id;
    }

    ComponentManager();

    template<typename T>
    void load() {
        systems.insert({getTypeId<T>(), std::make_unique<ComponentSystem<T>>()});
    }

    template<typename T>
    void unload() {
        systems.at(getTypeId<T>()).reset();
    }

    void update();

    Component& add(u32 type) {
        return systems[type]->add();
    }

    template<typename T>
    u32 count() {
        return systems.at(getTypeId<T>())->count();
    }

    u32 count(u32 type) {
        return systems.at(type)->count();
    }

    void del(Component::Key& key) {
        systems.at(key.type)->del(key.id);
    }

    template<typename T>
    void del(u32 id) {
        systems.at(getTypeId<T>())->del(id);
    }

    template<typename T>
    Component& get(u32 id) {
        return systems.at(getTypeId<T>())->get(id);
    }

    Component& get(Component::Key& key) {
        return systems.at(key.type)->get(key.id);
    }

    template<typename T>
    auto& getSystem() {
        return static_cast<ComponentSystem<T>&>(*systems.at(getTypeId<T>()));
    }

};

}

#include "entity.hpp"
