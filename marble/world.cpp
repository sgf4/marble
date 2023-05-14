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

#include "world.hpp"
#include "entity.hpp"
#include "component.hpp"
#include "components/camera.hpp"
#include "components/shader.hpp"
#include "components/transform2d.hpp"
#include "components/model.hpp"
#include "components/light.hpp"
#include "window.hpp"

using namespace ME;

World::World() {
    componentManager = std::make_unique<ComponentManager>();
    entities.reserve(100);

    WINDOW.hideCursor();
    current = this;

    // Basic components
    CM.load<Transform>();
    CM.load<Shader>();
    CM.load<Transform2D>();
    CM.load<Camera>();
    CM.load<Model>();
    CM.load<Light>();
    //CM.load<Gui>();

    camera = addEntity().getRef();
    getCamera().addComponent<Camera>().setControl(true);

    addEntity().addComponent<Light>();
}

u32 World::getEntityCount() {
    return entities.size();
}

void World::updateWorld() {
    current = this;
    componentManager->update();
    time.update();
}

Entity& World::addEntity() {
    Entity& e = entities.emplace_back();
    e.id = entities.size()-1;
    return e;
}

Entity& World::getEntity(u32 id) {
    return entities[id];
}

void World::delEntity(u32 id) {
    Entity& e = entities[id];
    e.destroy();
    e = std::move(*--entities.end());
    entities.pop_back();
}


void World::delEntity(Entity& e) {
    delEntity(e.id);
}

void World::updateUniforms(GL::Shader& s) {
    glUseProgram(s);
    glUniform1f(s.getUniform("utime"), time.elapsed);
    glUseProgram(0);
}


World::~World() {

}
