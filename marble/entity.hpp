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

#include "world.hpp"
#include "component.hpp"
#include <array>
#include <unordered_map>

#define ENTITY getEntity()

namespace ME {

class Entity {
    using ComponentKeys = std::array<Component::Key, 8>;
    ComponentKeys componentKeys;
    //std::unique_ptr<std::unordered_map<std::type_index, u32>> extendedComponentKeys;
    std::weak_ptr<Entity*> ref;
    u32 nComponents {};
public:

    Entity() {}
    Entity(Entity&) = delete;
    Entity(Entity&& other);

    Entity& operator=(Entity&&);

    u32 id;

    Component& addComponent(u32 type);

    template<typename T>
    T& addComponent() {
        CM.load<T>();
        return static_cast<T&>(addComponent(CM.getTypeId<T>()));
    }

    void delComponent(u32 type);
    void delComponent(Component::Key& type);

    template<typename T>
    void delComponent() {
        delComponent(CM.getTypeId<T>());
    }

    bool hasComponent(u32 type);

    template<typename T>
    bool hasComponent() {
        return hasComponent(CM.getTypeId<T>());
    }

    Component& getComponent(u32 type);

    template<typename T>
    T& getComponent() {
        return static_cast<T&>(getComponent(CM.getTypeId<T>()));
    }

    template<typename T>
    T* getOptionalComponent() {
        try {
            return getComponent<T>();
        } catch (...) {
            return 0;
        }
    }

    template<typename... Ts>
    auto getComponents() {
        return std::tie(getComponent<Ts>()...);
    }

    Component::Key& getComponentKey(u32 type);

    template<typename T>
    Component::Key& getComponentKey() {
        return getComponentKey(CM.getTypeId<T>());
    }

    ComponentKeys::iterator getComponentKeyIt(u32 type);

    auto getId() { return id; }

    operator u32() { return getId(); }

    void destroy();

    std::shared_ptr<Entity*> getRef();
};

// Shortcuts for Component
template<typename T>
T& Component::addComponent() {
    return getEntity().addComponent<T>();
}

template<typename T>
bool Component::hasComponent() {
    return getEntity().hasComponent<T>();
}

template<typename T>
T& Component::getComponent() {
    return getEntity().getComponent<T>();
}

template<typename T>
auto Component::getOptionalComponent() {
    return getEntity().getOptionalComponent<T>();
}

template<typename T>
void Component::delComponent() {
    getEntity().delComponent<T>();
}

}
