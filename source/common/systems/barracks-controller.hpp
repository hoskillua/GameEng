#pragma once

#include "../ecs/world.hpp"
#include "../components/player-controller.hpp"

#include "../application.hpp"


#include <stdlib.h>     
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>





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
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break

            /*
            
            if(player collided with barracks && !is_money_taken)
            {
                player_money+=money;
                is_money_taken = true;

                chnage_color_barracks(red);
            }
            */
            
            
        }
    
    
    
    
    
    
    }







}






