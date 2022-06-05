#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", -10000.0f);
        far = data.value("far", 10000.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //DONE: (Req 7) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        glm::vec3 eye = glm::vec3(0,0,0);
        glm::vec3 center = glm::vec3(0,0,-1);
        glm::vec3 up = glm::vec3(0,1,0);
        glm::vec3 eyePos = glm::vec3(M * glm::vec4(eye, 1));
        glm::vec3 centerPos = glm::vec3(M * glm::vec4(center, 1));
        glm::vec3 upDir = glm::vec3(M * glm::vec4(up, 0));
        return glm::lookAt(eyePos, centerPos, upDir);
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //DONE: (Req 7) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        float aspectRatio = (float)viewportSize.x / (float)viewportSize.y;
        if(cameraType == CameraType::PERSPECTIVE){
            return glm::perspective(
                fovY, // field of view in y direction
             aspectRatio, // aspect ratio
             near, // near plane
              far // far plane
              );
        } else {
            return glm::ortho(
                -orthoHeight * aspectRatio, // left
                orthoHeight * aspectRatio, // right
                -orthoHeight, // bottom
                orthoHeight, // top
                near, // near plane
                far // far plane
                );
        }
    }
       
    
}