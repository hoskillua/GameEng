#pragma once

#include "../ecs/component.hpp"
#include <stdlib.h>     

#include <glm/mat4x4.hpp>



namespace our 
{

    class BarracksControllerComponent : public Component
    {

        public:
            float radius=20.0f;
            bool is_money_taken=false;
            int money=rand()%100;
            static std::string getID() { return "barracks controller"; }

    void deserialize(const nlohmann::json& data){}
    };






}




