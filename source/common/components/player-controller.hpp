#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

    class PlayerControllerComponent : public Component {
    public:
        float Vspeed = 0.0f;
        float VspeedMax = 3.0f;
        float VAccel = 1.0f;
        float Aspeed = 0.0f;
        float AspeedMax = 3.0f;
        float AAccel = 1.0f;
        float radius = 5.0f;
        float health = 0.1f;
        int money=0;
        bool flag_won=false;
        // The ID of this component type is "Player"
        static std::string getID() { return "Player Controller"; }

        // Reads camera parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}