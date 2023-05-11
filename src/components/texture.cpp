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

// #include <marble/gl/texture.hpp>
// #include <Game.hpp>
// #include <Window.hpp>

// void Texture::init() {
//     addComponent<Gui>();
// }

// void Texture::update() {
//     if (!m_texture) return;

//     ComponentSystem<Texture>& system = getSystem<Texture>();
//     GL::Shader& sTexture = system.sTexture;
//     glUseProgram(sTexture);
//     glBindTexture(GL_TEXTURE_2D, *m_texture);
//     glBindVertexArray(GL::instance->getSquareVAO());
//     glUniform2f(sTexture.getUniform("utex_offset"), 0, 0);
//     glUniform4f(sTexture.getUniform("ucolor"), 1, 1, 1, 1);
//     glUniform2f(sTexture.getUniform("utex_size"), 1, 1);
//     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//     glUseProgram(0);

// }

// // void Texture::update(Transform2D& transform) {
// //     auto& s = game->getShader("texture");
// //     transform.updateUniforms(s);

// //     glUseProgram(s);
// //     glBindTexture(GL_TEXTURE_2D, *m_texture);
// //     glUniform2f(s.getUniform("ufit"), (float)m_texture->getWidth()/WINDOW_RX, (float)m_texture->getHeight()/WINDOW_RY);
// //     glUniform2f(s.getUniform("utex_offset"), 0, 0);
// //     glUniform2f(s.getUniform("utex_size"), 1, 1);
// //     glUniform4f(s.getUniform("ucolor"), 1, 1, 1, 1);
// //     glBindVertexArray(GL::instance->getSquareVAO());
// //     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
// //     glBindVertexArray(0);
// //     glUseProgram(0);
// // }

// // void Texture::update(Transform& transform, Camera& camera) {
// //     auto& s = game->getShader("texture3d");
// //     transform.updateUniforms(s);
// //     camera.updateUniforms(s);

// //     glUseProgram(s);
// //     glBindTexture(GL_TEXTURE_2D, *m_texture);
// //     glUniform2f(s.getUniform("ufit"), (float)m_texture->getWidth()/m_texture->getHeight(), (float)1.0);
// //     glUniform2f(s.getUniform("utex_offset"), 0, 0);
// //     glUniform2f(s.getUniform("utex_size"), 1, 1);
// //     glUniform4f(s.getUniform("ucolor"), 1, 1, 1, 1);
// //     glBindVertexArray(GL::instance->getSquareVAO());
// //     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
// //     glBindVertexArray(0);
// //     glUseProgram(0);
// // }
