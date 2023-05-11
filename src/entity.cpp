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

#include <marble/entity.hpp>
#include <algorithm>

using namespace ME;

void Entity::updateId(u32 i) {
    id = i;
    if (auto r = ref.lock()) *r = i;
    std::for_each_n(componentKeys.begin(), nComponents, [&] (Component::Key& k) {
        CM.get(k).eId = id;
    });
}

Component& Entity::addComponent(u32 type) {
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

Component::Key& Entity::getComponentKey(u32 type) {
    auto it = getComponentKeyIt(type);
    if (it == componentKeys.end()) throw std::string("Component with key ") + std::to_string(type) + " not found";
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
        delComponent(componentKeys[0].type);
    }
}

std::shared_ptr<u32> Entity::getRef() {
    if (ref.expired()) {
        auto ref = std::make_shared<u32>(id);
        ref = ref;
        return ref;
    }
    return ref.lock();
}
