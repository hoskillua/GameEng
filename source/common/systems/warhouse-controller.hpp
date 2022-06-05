#pragma once

#include "../ecs/world.hpp"
#include "../components/player-controller.hpp"
#include "../components/barracks-controller.hpp"
#include "../components/warhouse-controller.hpp"

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

    class WarhouseControllerSystem
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

            WarhouseControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                controller = entity->getComponent<WarhouseControllerComponent>();
                if (controller)
                {
                    Entity *entity = controller->getOwner();
                    glm::vec3 &position = entity->localTransform.position;

                    Entity *player_entity = getPlayerEntity(world);

                    glm::vec3 playerPos = player_entity->localTransform.position;

                    if (glm::distance(playerPos, position) < (controller->radius+10.f)  && player_entity->getComponent<PlayerControllerComponent>()->flag_won)
                    {
                        app->changeState("win-state");
                    }
                    // else if(glm::distance(playerPos, position) < (controller->radius+10.f))
                    // {
                    //     std::cout << "You have no money now" << std::endl;
                    // }

                    
                }
            }
        }


        Entity *getPlayerEntity(World *world)
        {

            for (auto entity : world->getEntities())
            {
                if (entity->name == "player")
                {
                    return entity;
                }
            }
            return nullptr;
        }

        void exit()
        {
        }
    };

};
