#pragma once

#include "../ecs/world.hpp"
#include "../components/player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class PlayerControllerSystem
    {
        Application *app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            PlayerControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                controller = entity->getComponent<PlayerControllerComponent>();
                if (controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = controller->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            // if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked){
            //     app->getMouse().lockMouse(app->getWindow());
            //     mouse_locked = true;
            // // If the left mouse button is released, we unlock and unhide the mouse.
            // } else if(!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
            //     app->getMouse().unlockMouse(app->getWindow());
            //     mouse_locked = false;
            // }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;
            glm::vec3 positionPrev = position;
            glm::vec3 rotationPrev = rotation;



            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            // if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)){
            //     glm::vec2 delta = app->getMouse().getMouseDelta();
            //     rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
            //     rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            // }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            // if(rotation.x < -glm::half_pi<float>() * 0.99f) rotation.x = -glm::half_pi<float>() * 0.99f;
            // if(rotation.x >  glm::half_pi<float>() * 0.99f) rotation.x  = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            // rotation.y = glm::wrapAngle(rotation.y);

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            // glm::mat4 matrix = glm::eulerAngleY(entity->localTransform.rotation.y);
            // glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
            //          up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
            //          right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            // glm::vec3 current_sensitivity = controller->positionSensitivity;
            //  If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            // if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;

            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT))
            {
                controller->Aspeed = std::min(controller->Aspeed + controller->AAccel * deltaTime, controller->AspeedMax);
            }
            else if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
            {
                controller->Aspeed = std::max(controller->Aspeed - controller->AAccel * deltaTime, -controller->AspeedMax);
            }
            else if (controller->Aspeed > 0.01f)
            {
                controller->Aspeed = std::max(controller->Aspeed - controller->AAccel * deltaTime, 0.0f);
            }
            else if (controller->Aspeed < -0.01f)
            {
                controller->Aspeed = std::min(controller->Aspeed + controller->AAccel * deltaTime, 0.0f);
            }
            else
            {
                controller->Aspeed = 0.0f;
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_UP))
            {
                controller->Vspeed = std::min(controller->Vspeed + controller->VAccel * deltaTime, controller->VspeedMax);
                rotation.y += controller->Vspeed * controller->Aspeed * deltaTime;
                position.z += controller->Vspeed * deltaTime * cos(rotation.y);
                position.x += controller->Vspeed * deltaTime * sin(rotation.y);
            }
            else if (app->getKeyboard().isPressed(GLFW_KEY_DOWN))
            {
                controller->Vspeed = std::max(controller->Vspeed - controller->VAccel * deltaTime, -controller->VspeedMax);
                rotation.y += controller->Vspeed * controller->Aspeed * deltaTime;
                position.z += controller->Vspeed * deltaTime * cos(rotation.y);
                position.x += controller->Vspeed * deltaTime * sin(rotation.y);
            }
            else if (controller->Vspeed > 0.01f)
            {
                controller->Vspeed = std::max(controller->Vspeed - controller->VAccel * deltaTime, 0.0f);
                rotation.y += controller->Vspeed * controller->Aspeed * deltaTime;
                position.z += controller->Vspeed * deltaTime * cos(rotation.y);
                position.x += controller->Vspeed * deltaTime * sin(rotation.y);
            }
            else if (controller->Vspeed < -0.01f)
            {
                controller->Vspeed = std::min(controller->Vspeed + controller->VAccel * deltaTime, 0.0f);
                rotation.y += controller->Vspeed * controller->Aspeed * deltaTime;
                position.z += controller->Vspeed * deltaTime * cos(rotation.y);
                position.x += controller->Vspeed * deltaTime * sin(rotation.y);
            }
            else
            {
                controller->Vspeed = 0.0f;
            }
            for (auto entity : world->getEntities())
            {
                if(entity != controller->getOwner() && entity->name != "ground" && entity->name != "bullet")
                {
                    glm::vec3 entityPos = entity->localTransform.position;
                    if(glm::distance(entityPos, position) < controller->getOwner()->radius + entity->radius)

                    {
                        position = positionPrev;
                        rotation = rotationPrev;
                        break;
                    }
                }
            }
           // if health is zero change the sence
            if(controller->health <= 0)
            {
               app->changeState("texture-test");
            }

            // A & D moves the player left or right
            // if(app->getKeyboard().isPressed(GLFW_KEY_LEFT)) position += right * (deltaTime * current_sensitivity.x);
            // if(app->getKeyboard().isPressed(GLFW_KEY_RIGHT)) position -= right * (deltaTime * current_sensitivity.x);
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
        }
    };

}
