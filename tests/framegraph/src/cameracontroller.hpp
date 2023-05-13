/**
 *  xEngine - C++ game engine library
 *  Copyright (C) 2023  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef XENGINE_CAMERACONTROLLER_HPP
#define XENGINE_CAMERACONTROLLER_HPP

#include "xng/xng.hpp"

using namespace xng;

class CameraController {
public:
    float rotationSpeed = 5;
    float movementSpeed = 10;

    CameraController(Transform &transform, Input &input)
            : transform(transform), input(input) {}

    void update(DeltaTime delta) {
        if (input.getMouse().getButtonDown(xng::MOUSE_BUTTON_RIGHT)) {
            lockCursor = true;
            input.setMouseCursorMode(xng::Input::CURSOR_DISABLED);
        } else if (!input.getMouse().getButton(MOUSE_BUTTON_RIGHT)) {
            lockCursor = false;
            input.setMouseCursorMode(xng::Input::CURSOR_NORMAL);
        }
        if (lockCursor) {
            transform.applyRotation(Quaternion(Vec3f(-input.getMouse().positionDelta.y * rotationSpeed * delta, 0, 0)),
                                    false);
            transform.applyRotation(Quaternion(Vec3f(0, -input.getMouse().positionDelta.x * rotationSpeed * delta, 0)),
                                    true);
        }

        Vec3f movement;
        if (input.getKeyboard().getKey(KEY_W)) {
            movement.y = -1;
        } else if (input.getKeyboard().getKey(xng::KEY_S)) {
            movement.y = 1;
        }

        if (input.getKeyboard().getKey(KEY_A)) {
            movement.x = 1;
        } else if (input.getKeyboard().getKey(xng::KEY_D)) {
            movement.x = -1;
        }

        if (input.getKeyboard().getKey(xng::KEY_SPACE)) {
            movement.z = 1;
        } else if (input.getKeyboard().getKey(xng::KEY_LCTRL)) {
            movement.z = -1;
        }

        if (movement.magnitude() > 0) {
            transform.setPosition(
                    transform.getPosition() + (transform.forward() * movement.y * movementSpeed * delta));
            transform.setPosition(
                    transform.getPosition() + (transform.left() * movement.x * movementSpeed * delta));
            transform.setPosition(transform.getPosition() + Vec3f(0, movement.z * movementSpeed * delta, 0));
        }
    }

private:
    Transform &transform;
    Input &input;

    bool lockCursor = false;
};

#endif //XENGINE_CAMERACONTROLLER_HPP
