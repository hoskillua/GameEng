#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

    class PlayerControllerComponent : public Component {
    public:
        float Hspeed = 0.0f;
        float Vspeed = 0.0f;
        float HspeedMax = 3.0f;
        float VspeedMax = 3.0f;
        float HAccel = 1.0f;
        float VAccel = 1.0f;

        // The ID of this component type is "Player"
        static std::string getID() { return "Player Controller"; }

        // Reads camera parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}