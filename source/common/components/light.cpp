#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include "../deserialize-utils.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>

namespace our {
    // Reads Light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string lightTypeStr = data.value("lightType", "directional");
        if(lightTypeStr == "spot"){
            type = LightType::SPOT;
        } else if(lightTypeStr == "point") {
            type = LightType::POINT;
        } else {
            type = LightType::DIRECTIONAL;
        }
        
        diffusion = data.value("diffusion", diffusion);
        
        specular = data.value("specular", specular);
        attenuation = data.value("attenuation", attenuation);
        cone_angles = data.value("cone_angles", cone_angles);
    }      
    
}