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
        float damage = 0.05f;
        float velocity;
        std::string postprocess = "assets/shaders/postprocess/damage.frag";
        float postprocessTime = 0.15f;
        float postprocessElapsedTime = 0.0f;
        bool postprocessEnabled = false;
        // The ID of this component type is "Movement"
        static std::string getID() { return "Bullet Controller"; }
        // Reads linearVelocity & angularVelocity from the given json object
        void deserializeDynamic(
            glm::vec3 startPosition,
            glm::vec3 endPosition,
            float damage,
            float velocity)
        {
            this->startPosition = startPosition;
            this->endPosition = endPosition;
            this->damage = damage;
            this->velocity = velocity;
        }
        void deserialize(const nlohmann::json &data) {}
    };
}
