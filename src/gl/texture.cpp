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

#include <marble/gl/texture.hpp>
#include <HLib/stb_image.hpp>
#include <marble/gl/gl.hpp>

using namespace ME::GL;

Texture::Texture() {
    glGenTextures(1, &id);
}

Texture::Texture(unsigned char* fdata, u32 size) : Texture() {
    int c;
    int w;
    int h;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load_from_memory(fdata, size, &w, &h, &c, 0);

    width = w;
    height = h;

    // Upload
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    int format = GL_RGBA;
    switch (c) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}


Texture::~Texture() {
    glDeleteTextures(1, &id);
}
