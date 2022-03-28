#pragma once

#include <application.hpp>

class ClearColorState: public our::State {
    // onInitialize() function is called once before the state starts
    void onInitialize() override {
        //DONE: Read the color from the configuration file and use it to set the clear color for the window
        //HINT: you can the configuration for the whole application using "getApp()->getConfig()"
        //To see how the clear color is written in the json files, see "config/blue-screen.json"

        //getting the configuration from the json file
        const auto& config = getApp()->getConfig()["scene"]["clear-color"];
        // Tell openGl the settings (The color used while clearing the window)
        glClearColor((float)config.value("r",1.0),(float)config.value("g",1.0),(float)config.value("b",1.0),(float)config.value("a",1.0));
        //To know how read data from a nlohmann::json object, 
        //look at the following documentation: https://json.nlohmann.me/features/element_access/
    }

    // onDraw(deltaTime) function is called every frame 
    void onDraw(double deltaTime) override {
        //At the start of frame we want to clear the screen. Otherwise we would still see the results from the previous frame.
        glClear(GL_COLOR_BUFFER_BIT);
    }
};