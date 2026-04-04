#include <Camera.h>
#include <Character.h>

Camera::Camera(TDT4102::AnimationWindow& window)
    : window(window) {}

void Camera::follow(const Character& character) {
    x = character.getX() - static_cast<float>(window.width()) / 2.f + character.getWidth() / 2.f;
    y = character.getY() - static_cast<float>(window.height()) / 2.f + character.getHeight() / 2.f;
}

TDT4102::Point Camera::worldToCamera(float worldX, float worldY) const {
    return {static_cast<int>(worldX - x), static_cast<int>(worldY - y)};
}

float Camera::getX() const {
    return x;
}

float Camera::getY() const {
    return y;
}