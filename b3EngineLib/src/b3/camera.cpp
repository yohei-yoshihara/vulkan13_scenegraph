#include "camera.hpp"

namespace b3 {

Camera Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center) {
  Camera cam;
  cam.m_position = eye;
  glm::vec3 dir = glm::normalize(center - eye);
  cam.m_yaw = glm::degrees(std::atan2(dir.y, dir.x));
  float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
  cam.m_pitch = glm::degrees(std::atan2(dir.z, len));
  return cam;
}

void Camera::handleMouseEvent(GLFWwindow *window, double x, double y) {
  if (!m_gotFirstMousePosition) {
    m_gotFirstMousePosition = true;
  }
  float dx = (x - m_lastX) * m_sensitivity;
  float dy = (y - m_lastY) * m_sensitivity;

  m_yaw += -dx;
  m_pitch -= dy;

  if (m_pitch > 89.0f) {
    m_pitch = 89.0f;
  }
  if (m_pitch < -89.0f) {
    m_pitch = -89.0f;
  }
  m_lastX = x;
  m_lastY = y;
}

void Camera::updateCameraMovement(GLFWwindow *window, float dt) {
  glm::vec3 front;
  front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  front.y = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  front.z = std::sin(glm::radians(m_pitch));
  front = glm::normalize(front);

  glm::vec3 up(0.0f, 0.0f, 1.0f);
  glm::vec3 right = glm::normalize(glm::cross(front, up));

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_position += front * m_speed * dt;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_position -= front * m_speed * dt;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_position -= right * m_speed * dt;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_position += right * m_speed * dt;
  }
}

glm::mat4 Camera::getCameraView() const {
  glm::vec3 front;
  front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  front.y = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  front.z = std::sin(glm::radians(m_pitch));

  glm::vec3 center = m_position + glm::normalize(front);

  return glm::lookAt(m_position, center, glm::vec3(0.f, 0.f, 1.f));
}

} // namespace b3