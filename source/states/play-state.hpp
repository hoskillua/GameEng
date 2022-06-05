#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/player-controller.hpp>
#include <systems/barracks-controller.hpp>
#include <systems/warhouse-controller.hpp>

#include <systems/cannon-controller.hpp>
#include <systems/bullet-controller.hpp>
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
    our::WarhouseControllerSystem warhouseController;
    our::CannonControllerSystem cannonController;
    our::BulletControllerSystem bulletController;
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
        warhouseController.enter(getApp());
        cannonController.enter(getApp());
        bulletController.enter(getApp());
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
        warhouseController.update(&world, (float)deltaTime);
        cannonController.update(&world, (float)deltaTime);
        bulletController.update(&world, (float)deltaTime, &renderer);
        // And finally we use the renderer system to draw the scene
        world.deleteMarkedEntities();
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
        warhouseController.exit();
        cannonController.exit();
        bulletController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void onImmediateGui() override
    {
        our::CameraComponent *camera = nullptr;
        our::FreeCameraControllerComponent *controller = nullptr;
        our::PlayerControllerComponent *player = nullptr;
        // We use ImGui to draw some debug information
        ImGui::Begin("Debug");
        for (auto entity : world.getEntities())
        {
            if (!camera)
                camera = entity->getComponent<our::CameraComponent>();
            if (!controller)
                controller = entity->getComponent<our::FreeCameraControllerComponent>();
            if (!player)
                player = entity->getComponent<our::PlayerControllerComponent>();
            if (camera && controller && player)
                break;
        }
        if(!camera || !controller || !player)
            return;

        ImGui::DragFloat3("Camera position", &camera->getOwner()->localTransform.position.x);
        ImGui::DragFloat3("Camera speed", &controller->positionSensitivity.x);
        ImGui::DragFloat("Camera speedup factor", &controller->speedupFactor);
        ImGui::End();
        ImGui::Begin("health",0,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove);
        // change window position
        ImGui::SetWindowPos(ImVec2(950,10));
        // change window size
        ImGui::SetWindowSize(ImVec2(300,50));
        // colored progressbar
        ImGui::ProgressBar(player->health);
        ImGui::End();
        ImGui::Begin("money",0,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove);
        // change window position
        ImGui::SetWindowPos(ImVec2(950,50));
        // change window size
        ImGui::SetWindowSize(ImVec2(300,20));
        // add styled text to the window
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Money: %d", player->money);
        ImGui::End();
    }
};