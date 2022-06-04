#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{
    // This component denotes that the MovementSystem will move the owning entity by a certain  angular velocity.
    class BulletControllerComponent : public Component
    {
    public:
        glm::vec3 startPosition;
        glm::vec3 endPosition;
        float velocity;
        // The ID of this component type is "Movement"
        static std::string getID() { return "Bullet Controller"; }
        // Reads linearVelocity & angularVelocity from the given json object
        void deserializeDynamic(
            glm::vec3 startPosition,
            glm::vec3 endPosition,
            float velocity)
        {
            this->startPosition = startPosition;
            this->endPosition = endPosition;
            this->velocity = velocity;
        }
        void deserialize(const nlohmann::json &data) {}
    };
}
