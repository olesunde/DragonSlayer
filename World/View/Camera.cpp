#include <Camera.h>
#include <Character.h>

void Camera::follow(const TDT4102::AnimationWindow& window, const Character& character) {
    x = character.getX() - static_cast<float>(window.width()) / 2.f + character.getWidth() / 2.f;
    y = character.getY() - static_cast<float>(window.height()) / 2.f + character.getHeight() / 2.f;
}

TDT4102::Point Camera::worldToCamera(float worldX, float worldY) const {
    return {static_cast<int>(worldX - x), static_cast<int>(worldY - y)};
}