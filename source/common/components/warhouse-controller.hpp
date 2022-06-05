#pragma once

#include "../ecs/component.hpp"
#include <stdlib.h>

#include <glm/mat4x4.hpp>



namespace our
{

    class WarhouseControllerComponent : public Component
    {

        public:
            float radius=30.0f;
            static std::string getID() { return "warhouse controller"; }

    void deserialize(const nlohmann::json& data){}
    //deconstructor

    };
}




