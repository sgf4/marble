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
#include "../component.hpp"
#include "../gl/gl.hpp"

namespace ME {

class Gui : public Component {
public:

    enum Align2D : u8 {
        TOP_LEFT,
        TOP_CENTERED,
        TOP_RIGHT,

        MIDDLE_LEFT,
        MIDDLE_CENTERED,
        MIDDLE_RIGHT,

        BOTTOM_LEFT,
        BOTTOM_CENTERED,
    };

    void init();
    void update();
};

}
