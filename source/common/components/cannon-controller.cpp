#include "cannon-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads angularVelocity from the given json object
    void CannonControllerComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;
        angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));
        range = data.value("range", range);
        damage = data.value("damage", damage);
        fireRate = data.value("fireRate", fireRate);
        reloadTime = data.value("reloadTime", reloadTime);
    }
}