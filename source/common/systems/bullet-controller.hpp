#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/cannon-controller.hpp"
#include "../components/bullet-controller.hpp"
#include "../systems/forward-renderer.hpp"

#include "../application.hpp"

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>

namespace our
{

    class BulletControllerSystem
    {
        Application *app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime, ForwardRenderer *renderer)
        {

            BulletControllerComponent *controller = nullptr;
            MeshRendererComponent *playermesh = nullptr;
            PlayerControllerComponent *playercomponent = nullptr;

            for (auto entity : world->getEntities())
            {
                if (entity->name == "player")
                {
                    playermesh = entity->getComponent<MeshRendererComponent>();
                    playercomponent = entity->getComponent<PlayerControllerComponent>();
                }
            }

            for (auto entity : world->getEntities())
            {
                controller = entity->getComponent<BulletControllerComponent>();

                if (controller)
                {
                    Entity *entity = controller->getOwner();
                    glm::vec3 &position = entity->localTransform.position;
                    if (glm::distance(position, controller->endPosition) > 0.5f)
                    {
                        glm::vec3 direction = glm::normalize(controller->endPosition - controller->startPosition);
                        position += direction * controller->velocity * deltaTime;
                    }
                    else if (!controller->postprocessEnabled){
                        playercomponent->health -= controller->damage;
                        renderer->updatePostprocess(controller->postprocess);
                        controller->postprocessEnabled = true;
                    }
                    if(controller->postprocessEnabled){
                        if(controller->postprocessElapsedTime > controller->postprocessTime)
                        {
                            renderer->updatePostprocess();
                            controller->postprocessEnabled = false;
                            world->markForRemoval(entity);
                        }
                        else{
                            controller->postprocessElapsedTime += 0.01f;
                        }
                    }
                }
            }
        }

        void exit()
        {
        }
    };

};
