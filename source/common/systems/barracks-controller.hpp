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

#include<iostream>



namespace our
{


    class BarracksControllerSystem
    {
        Application *app; // The application in which the state runs
    
    public:
        int money;
        bool is_money_taken;
        BarracksControllerSystem()
        {
            money = rand() % 100;
            is_money_taken = false;
        }

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
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = controller->getOwner();
            glm::vec3 &position = entity->localTransform.position;


            Entity *player_entity =getPlayerEntity(world);
            
            glm::vec3 playerPos = player_entity->localTransform.position;


            if(glm::distance(playerPos, position) < controller->radius  &&  !is_money_taken)
            {

                is_money_taken = true;
                //todo: add money to player
                //player_entity->getComponent<PlayerControllerComponent>()->money += money;
                std::cout<<"You have taken "<<money<<"$"<<std::endl;
                //todo, change color of baracks to red

            }
        

            //todo 
            /*
            if(60 sec){
                is_money_taken = false;
                money = rand() % 100;
                change_color_to_normal();
            }
            */

        

            
            
            
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
    
        Entity* getPlayerEntity(World* world)
        {

            for (auto entity : world->getEntities())
            {   
                if(entity->name == "player")
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






