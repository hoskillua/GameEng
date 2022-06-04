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
            static std::string getID() { return "barracks controller"; }

    void deserialize(const nlohmann::json& data){}
    };






}




