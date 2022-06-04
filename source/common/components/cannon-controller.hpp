#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{
    // This component denotes that the MovementSystem will move the owning entity by a certain  angular velocity.
    class CannonControllerComponent : public Component
    {
    public:
        glm::vec3 angularVelocity = {0, 0, 0}; // Each frame, the entity should rotate as follows: rotation += angularVelocity * deltaTime
        float range = 50.0f; // The range of the cannon
        float damage = 10.0f; // The damage of the cannon
        float fireRate = 1.0f; // The fire rate of the cannon
        float reloadTime = 0.0f; // The reload time of the cannon

        // The ID of this component type is "Movement"
        static std::string getID() { return "CannonMovement"; }
        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}
