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

#include "entity.hpp"
#include <algorithm>

using namespace ME;

Entity::Entity(Entity&& other) {
    id = other.id;
    *this = std::move(other);
}

Entity& Entity::operator=(Entity&& other) {
    componentKeys = std::move(other.componentKeys);
    ref = std::move(other.ref);
    nComponents = other.nComponents;

    if (!ref.expired()) {
        *(ref.lock()) = this;
    }

    std::for_each_n(componentKeys.begin(), nComponents, [&] (Component::Key& k) {
        CM.get(k).eId = id;
    });

    return *this;
}

Component& Entity::addComponent(u32 type) {
    try { return getComponent(type); }
    catch (...) {}
    Component& c = CM.add(type);
    c.eId = id;
    componentKeys[nComponents++] = {type, c.id};
    c.init();
    return c;
}

void Entity::delComponent(u32 type) {
    Component::Key& key = getComponentKey(type);
    delComponent(key);
}

void Entity::delComponent(Component::Key& key) {
    Component& c = CM.get(key);
    c.destroy();
    CM.del(key);

    if (CM.count(key.type) != key.id) c.getEntity().getComponentKey(key.type).id = key.id;
    key = componentKeys[--nComponents];
}

bool Entity::hasComponent(u32 type) {
    auto it = getComponentKeyIt(type);
    return it != componentKeys.begin()+nComponents;
}

Component& Entity::getComponent(u32 type) {
    return CM.get(getComponentKey(type));
}

Component::Key& Entity::getComponentKey(u32 type) {
    auto it = getComponentKeyIt(type);
    if (it == componentKeys.begin()+nComponents) throw std::string("Component with key ") + std::to_string(type) + " not found";
    return *it;
}

Entity::ComponentKeys::iterator Entity::getComponentKeyIt(u32 type) {
    return std::find_if(
        componentKeys.begin(),
        componentKeys.begin()+nComponents,
        [&] (Component::Key& k) {
            return k.type == type;
        }
    );
}

void Entity::destroy() {
    for (u32 i=0; i<nComponents; i++) {
        delComponent(componentKeys[0]);
    }
}

std::shared_ptr<Entity*> Entity::getRef() {
    if (ref.expired()) {
        auto r = std::make_shared<Entity*>(this);
        ref = r;
        return r;
    }
    return ref.lock();
}
