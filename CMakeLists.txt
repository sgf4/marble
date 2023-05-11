# Copyright 2023 sgf4
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.14)

project(marble VERSION 0.1)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -fsanitize=address")
set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

option(USE_PCH "Use precompiled headers (StdAfx.hpp)" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE MARBLE_SOURCES src/*.cpp src/*.hpp)

add_library(marble STATIC ${MARBLE_SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(marble PUBLIC OpenGL::GL)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(marble PUBLIC glfw)

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(marble PUBLIC ${GLM_LIBRARIES})

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(marble PUBLIC ${ASSIMP_LIBRARIES})

target_include_directories(marble PUBLIC include)
target_include_directories(marble PRIVATE src)

if (USE_PCH)
  target_precompile_headers(marble PUBLIC
    src/stdafx.hpp
  )
endif()
