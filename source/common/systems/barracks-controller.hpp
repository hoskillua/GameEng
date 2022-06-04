#pragma once

#include "../ecs/world.hpp"
#include "../components/player-controller.hpp"
#include "../components/barracks-controller.hpp"

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

    class BarracksControllerSystem
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

            BarracksControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                controller = entity->getComponent<BarracksControllerComponent>();
                if (controller)
                {
                    Entity *entity = controller->getOwner();
                    glm::vec3 &position = entity->localTransform.position;

                    Entity *player_entity = getPlayerEntity(world);

                    glm::vec3 playerPos = player_entity->localTransform.position;

                    if (glm::distance(playerPos, position) < controller->radius && !controller->is_money_taken)
                    {

                        // done: add money to player
                        controller->is_money_taken = true;
                        controller->money=rand()%100;
                        player_entity->getComponent<PlayerControllerComponent>()->money += controller->money;
                        std::cout << "player money=" << player_entity->getComponent<PlayerControllerComponent>()->money << std::endl;

                        // player_entity->getComponent<PlayerControllerComponent>()->money += money;
                        std::cout << "You have taken " << controller->money << "$" << std::endl;

                        //entity->localTransform.position = glm::vec3(-1000, -1000, -1000);
                        world->markForRemoval(entity);
                    }

                    
                }
            }
            world->deleteMarkedEntities();
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
        }

        // bool is_collided_with_player(BarracksControllerComponent *controller)
        // {
        //     Entity *player_entity =getPlayerEntity();
        //     glm::vec3 entityPos = player_entity->localTransform.position;
        //     if(glm::distance(entityPos, position) < controller->radius)
        //     {
        //         return true;
        //     }
        //     return false;
        // }

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
