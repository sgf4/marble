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
#include <chrono>
#include "integers.hpp"

class Time {
    std::chrono::time_point<std::chrono::steady_clock> m_start;

    u32 m_frameCount {};
    float m_nextSecond {1.f};


public:

    float elapsed {};
    float delta {};
    u32 fps {};

    Time() {
        m_start = std::chrono::steady_clock::now();
    }

    void update() {
        float elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now()-m_start).count();
        delta = elapsedSeconds-elapsed;
        elapsed = elapsedSeconds;

        if (elapsedSeconds >= m_nextSecond) {
            fps = m_frameCount;
            m_frameCount = 0;
            m_nextSecond = elapsedSeconds+1.f;
        }

        m_frameCount++;
    }
};
