#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/player-controller.hpp>
#include <systems/barracks-controller.hpp>
#include <systems/cannon-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::PlayerControllerSystem playerController;
    our::BarracksControllerSystem barracksController;
    our::CannonControllerSystem cannonController;
    our::MovementSystem movementSystem;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        playerController.enter(getApp());
        barracksController.enter(getApp());
        cannonController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        playerController.update(&world, (float)deltaTime);
        barracksController.update(&world, (float)deltaTime);
        cannonController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        playerController.exit();
        barracksController.exit();
        cannonController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void onImmediateGui() override
    {
        our::CameraComponent *camera;
        our::FreeCameraControllerComponent *controller;
        // We use ImGui to draw some debug information
        ImGui::Begin("Debug");
        for (auto entity : world.getEntities())
        {
            camera = entity->getComponent<our::CameraComponent>();
            controller = entity->getComponent<our::FreeCameraControllerComponent>();
            if (camera && controller)
                break;
        }

        ImGui::DragFloat3("Camera position", &camera->getOwner()->localTransform.position.x);
        ImGui::DragFloat3("Camera speed", &controller->positionSensitivity.x);
        ImGui::DragFloat("Camera speedup faactor", &controller->speedupFactor);

        ImGui::End();
    }
};