#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>


namespace our {

    // An enum that defines the type of the light (DIRECTIONAL, POINT, or SPOT) 
    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };
    
    // This component denotes how each component will reflect the light\
    //It should be sent to each shader used 
    class LightComponent : public Component {
    public:
        int type;
        glm::vec3 diffuse = {0,0,0};
        glm::vec3 specular = {0,0,0};
        glm::vec3 attenuation = {0,0,0};
        glm::vec3 cone_angles = {0,0,0};
    
        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads Light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
        
    };

}