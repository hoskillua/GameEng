#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    /**
     * @brief 
     * This function computes and returns a matrix that represents this transform
     * Remember that the order of transformations is: Scaling, Rotation then Translation
     * HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
     * We know that each transformation have a representation in glm to be mat4
     * Just pass it to those functions with 1.0f as the 4th dimentsion
     * 
     * @return glm::mat4 
     */
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 2) Write this function


        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
                              glm::yawPitchRoll(rotation[1],rotation[0],rotation[2]) *
                              glm::scale(glm::mat4(1.0f),scale);



        return transform;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}