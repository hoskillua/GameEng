#include "player-controller.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void PlayerControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        VspeedMax = data.value("VerticalSpeed", VspeedMax);
        VAccel = data.value("VerticalAcceleration", VAccel);
        AspeedMax = data.value("AngularSpeed", AspeedMax);
        AAccel = data.value("AngularAcceleration", AAccel);
        radius = data.value("radius", radius);
    }
}