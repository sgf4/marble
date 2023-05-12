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
#include <GL.hpp>
#include "Transform2D.hpp"
#include "Transform.hpp"

class Texture : public Component {
    GL::Texture* m_texture {};

public:
    void init();
    void update();

    Texture& setTexture(GL::Texture* texture) { m_texture = texture; return *this; }
// public:

//     // enum Fit : u8 {
//     //     SCALE,
//     //     COVER,
//     //     CONTAIN
//     // };

//     Texture(GL::Texture& t);

//     void update(Transform2D& transform);
//     void update(Transform& transform, Camera& camera = WORLD.getCamera());
};

template<>
struct ComponentSystem<Texture> : public BasicComponentSystem<Texture> {
    GL::EmbedShader<"gui_texture"> sTexture;
};
