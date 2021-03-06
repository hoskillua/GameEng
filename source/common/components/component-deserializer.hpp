#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "light.hpp"
#include "player-controller.hpp"
#include "barracks-controller.hpp"
#include "warhouse-controller.hpp"
#include "cannon-controller.hpp"

namespace our {

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json& data, Entity* entity){
        std::string type = data.value("type", "");
        Component* component = nullptr;
        if(type == CameraComponent::getID()){
            component = entity->addComponent<CameraComponent>();
        } else if (type == MeshRendererComponent::getID()) {
            component = entity->addComponent<MeshRendererComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        } else if (type == PlayerControllerComponent::getID()) {
            component = entity->addComponent<PlayerControllerComponent>();
        } else if (type == LightComponent::getID()) {
            component = entity->addComponent<LightComponent>();
        } else if (type == BarracksControllerComponent::getID()) {
            component = entity->addComponent<BarracksControllerComponent>();
        } else if (type == CannonControllerComponent::getID()) {
            component = entity->addComponent<CannonControllerComponent>();
        } else if (type == WarhouseControllerComponent::getID()) {
            component = entity->addComponent<WarhouseControllerComponent>();
        }

        if(component) component->deserialize(data);
    }

}