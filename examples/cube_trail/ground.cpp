#include "ground.hpp"

void Ground::create(GLuint program, GLint modelMatrixLoc, GLint colorLoc,
                    GLint normalMatrixLoc, glm::mat4 viewMatrix, float scale,
                    int N) {
  // Unit quad on the xz plane
  m_vertices = {{
      {.position = {+0.5f, 0.0f, -0.5f}, .normal{0.0f, 1.0f, 0.0f}}, // V1
      {.position = {-0.5f, 0.0f, -0.5f}, .normal{0.0f, 1.0f, 0.0f}}, // V2
      {.position = {+0.5f, 0.0f, +0.5f}, .normal{0.0f, 1.0f, 0.0f}}, // V3
      {.position = {-0.5f, 0.0f, +0.5f}, .normal{0.0f, 1.0f, 0.0f}}  // V4
  }};

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  m_KaLoc = abcg::glGetUniformLocation(program, "Ka");
  m_KdLoc = abcg::glGetUniformLocation(program, "Kd");
  m_KsLoc = abcg::glGetUniformLocation(program, "Ks");

  // Load location of uniform variables of shader
  m_modelMatrixLoc = modelMatrixLoc;
  m_normalMatrixLoc = normalMatrixLoc;
  m_viewMatrix = viewMatrix;
  m_colorLoc = colorLoc;
  m_scale = scale;
  m_N = N;
}

// void surfacePaint(glm::mat4 model){
//   abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//   auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//   auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//   abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//   &normalMatrix[0][0]);

//   // Set color (checkerboard pattern)
//   abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//   abcg::glUniform1f(m_KaLoc, m_Ka);
//   abcg::glUniform1f(m_KdLoc, m_Kd);
//   abcg::glUniform1f(m_KsLoc, m_Ks);

//   abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// }

// void Ground::paint() {
//   abcg::glBindVertexArray(m_VAO);

//   //  grid 2N+1 x 2N+1 tiles on the xz plane, centered around the origin
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model =
//           glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0)); //
//           inoquo
//       model = glm::translate(
//           model, glm::vec3(x * m_scale, -0.5f * m_scale, z * m_scale));
//       // .5 para o ajuste vertical, o bloco inicializa no centro do
//       referencial
//       // e possui tamanho m_scale
//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   // x,z, -
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model = glm::rotate(model, glm::radians(180.0f),
//                           glm::vec3(1, 0, 0)); // Rotaciona em X
//       model = glm::translate(
//           model, glm::vec3(x * m_scale, (0.5f + 3.0f) * m_scale, z *
//           m_scale));
//       // ajuste do chão 0.5 + altura de 3 blocos
//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   // y,z, 0
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model = glm::rotate(model, glm::radians(90.0f),
//                           glm::vec3(0, 0, 1)); // Rotaciona em Z
//       model = glm::translate(model, glm::vec3(-(x + 1.0f + m_N) * m_scale,
//                                               1.5f * m_scale, z * m_scale));
//       // O sistema de coordenada é referenciado ao objeto, i.e., é alterado
//       com
//       // a rotação Ajuste após rotação, em eixos absolutos: x :: negativo de
//       m_N
//       // / 2 blocos ;  y :: negativo de um bloco

//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   // y,z, +
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model = glm::rotate(model, glm::radians(270.0f),
//                           glm::vec3(0, 0, 1)); // Rotaciona em Z
//       model = glm::translate(model, glm::vec3((x + 1.0f + m_N) * m_scale,
//                                               1.5f * m_scale, z * m_scale));
//       // Ajuste após rotação, em eixos absolutos:
//       // x ::  positivo de m_N / 2 blocos ;  y :: negativo de um bloco

//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   // x,y, 0
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model = glm::rotate(model, glm::radians(-90.0f),
//                           glm::vec3(1, 0, 0)); // Rotaciona em X
//       model = glm::translate(model, glm::vec3(x * m_scale, 1.5f * m_scale,
//                                               -(z + 1.0f + m_N) * m_scale));
//       // Ajuste após rotação, em eixos absolutos:
//       // z ::  negativo de m_N / 2 blocos ; y :: negativo de um bloco

//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   // x,y, +
//   for (auto const z : iter::range(-m_N, m_N + 1)) {
//     for (auto const x : iter::range(-m_N, m_N + 1)) {
//       // Set model matrix as a translation matrix
//       glm::mat4 model{1.0f};

//       model = glm::rotate(model, glm::radians(90.0f),
//                           glm::vec3(1, 0, 0)); // Rotaciona em X
//       model = glm::translate(model, glm::vec3(x * m_scale, 1.5f * m_scale,
//                                               (z + 1.0f + m_N) * m_scale));
//       // Ajuste após rotação, em eixos absolutos:
//       // z ::  positivo de m_N / 2 blocos ; y :: negativo de um bloco e m_N
//       para
//       // tirar do centro

//       model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

//       abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

//       auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
//       auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
//       abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
//                                &normalMatrix[0][0]);

//       // Set color (checkerboard pattern)
//       abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

//       abcg::glUniform1f(m_KaLoc, m_Ka);
//       abcg::glUniform1f(m_KdLoc, m_Kd);
//       abcg::glUniform1f(m_KsLoc, m_Ks);

//       abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//   }

//   abcg::glBindVertexArray(0);
// }

void Ground::drawTile(float angle, glm::vec3 axis, float xOffset, float yOffset,
                      float zOffset, float factorX, float factorY,
                      float factorZ) {

  for (auto const z : iter::range(-m_N, m_N + 1)) {
    for (auto const x : iter::range(-m_N, m_N + 1)) {
      glm::mat4 model{1.0f};

      model = glm::rotate(model, glm::radians(angle), axis);
      model =
          glm::translate(model, glm::vec3(factorX * (x + xOffset) * m_scale,
                                          factorY * yOffset * m_scale,
                                          factorZ * (z + zOffset) * m_scale));
      model = glm::scale(model, glm::vec3(m_scale, m_scale, m_scale));

      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      auto const modelViewMatrix{glm::mat3(m_viewMatrix * model)};
      auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
      abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
                               &normalMatrix[0][0]);

      abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);

      abcg::glUniform1f(m_KaLoc, m_Ka);
      abcg::glUniform1f(m_KdLoc, m_Kd);
      abcg::glUniform1f(m_KsLoc, m_Ks);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }
}

void Ground::paint() {
  abcg::glBindVertexArray(m_VAO);

  drawTile(0.0f,   glm::vec3(1, 0, 0), 0.0f, 0.5f, 0.0f, 1.0f, -1.0f, 1.0f);   // x,z, 0
  drawTile(180.0f, glm::vec3(1, 0, 0), 0.0f, 3.5f, 0.0f, 1.0f, 1.0f, 1.0f);  // x,z, +
  drawTile(90.0f,  glm::vec3(0, 0, 1), 1.0f+ m_N, 1.5f, 0.0f, -1.0f, 1.0f, 1.0f);  //  y,z, 0
  drawTile(270.0f, glm::vec3(0, 0, 1), 1.0f+ m_N, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f);  // y,z, +
  drawTile(-90.0f, glm::vec3(1, 0, 0), 0.0f, 1.5f, 1.0f + m_N, 1.0f, 1.0f, -1.0f); // x,y, 0
  drawTile(90.0f,  glm::vec3(1, 0, 0), 0.0f, 1.5f, 1.0f + m_N, 1.0f, 1.0f, 1.0f);   // x,y, +

  abcg::glBindVertexArray(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}