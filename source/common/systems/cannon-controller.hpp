#include "../ecs/world.hpp"
#include "../components/cannon-controller.hpp"
#include "../components/player-controller.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/bullet-controller.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
namespace our
{

    class CannonControllerSystem
    {
        Application *app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CannonControllerComponent *controller = nullptr;
            PlayerControllerComponent *playercomponent = nullptr;
            for (auto entity : world->getEntities())
            {
                playercomponent = entity->getComponent<PlayerControllerComponent>();
                if (playercomponent)
                {
                    break;
                }
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return

            if (!playercomponent)
                return;
            Entity *player = playercomponent->getOwner();
            for (auto entity : world->getEntities())
            {
                controller = entity->getComponent<CannonControllerComponent>();
                if (controller)
                {
                    Entity *entity = controller->getOwner();
                    // We get a reference to the entity's position and rotation
                    entity->localTransform.rotation.y += deltaTime * controller->angularVelocity.y;
                    if (entity->localTransform.rotation.y > glm::pi<float>() * 2.0f)
                        entity->localTransform.rotation.y -= glm::pi<float>() * 2.0f;
                    glm::vec3 &position = entity->localTransform.position;
                    glm::vec3 &rotation = entity->localTransform.rotation;
                    glm::vec3 &playerPosition = player->localTransform.position;
                    // Calculate the distance between the player and the cannon
                    float distance = glm::distance(playerPosition, position);
                    // std::cout << "Distance: " << distance << std::endl;
                    //  If the distance is greater than the cannon's range, we can't shoot
                    if (distance < controller->range)
                    {
                        // get direction to player
                        if (controller->timeElapsed > controller->reloadTime)
                        {
                            controller->timeElapsed = 0.0f;
                            FireBullet(position, playerPosition, rotation, controller->damage, world);
                        }
                        else
                        {
                            controller->timeElapsed += deltaTime * 0.3f;
                        }
                        glm::vec3 direction = glm::normalize(playerPosition - position);
                        // rotate y to direction
                        rotation.y = (rotation.y * 11 + glm::atan(direction.x, direction.z) + glm::pi<float>() / 2.0f) / 12;
                    }
                }
                // Get the entity that we found via getOwner of cannon (we could use controller->getOwner())
            }
        }
        void FireBullet(glm::vec3 position, glm::vec3 playerPosition, glm::vec3 rotation, float damage, World *world)
        {
            // Create a new entity
            Entity *bullet = world->add();
            bullet->name = "bullet";
            bullet->localTransform.scale = glm::vec3(-0.04f, 0.04f, 0.04f);
            bullet->localTransform.position = position;
            bullet->localTransform.position.y += 5.0f;
            bullet->localTransform.rotation = rotation;
            //bullet->localTransform.rotation.y -= glm::pi<float>() / 2.0f;
            // Add a BulletComponent to the entity
            MeshRendererComponent* mesh = bullet->addComponent<MeshRendererComponent>();
            mesh->deserializeDynamic("bullet","bullet");
            BulletControllerComponent* bulletController = bullet->addComponent<BulletControllerComponent>();
            bulletController->deserializeDynamic(bullet->localTransform.position, playerPosition + glm::vec3(0,5.0f,0), damage, 150.0f);
        }
        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
        }
    };
}