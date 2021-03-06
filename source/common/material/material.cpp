#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //DONE: (Req 6) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    

    // This function returns the type of material
    std::string Material::material_type()
    {
        return "";
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }



    // This function returns the type of material
    std::string TintedMaterial::material_type()
    {
        return "tint";
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        texture->bind();
        sampler->bind(0);
        shader->set("tex", 0);
          }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


    // This function returns the type of material
    std::string TexturedMaterial::material_type()
    {
        return "textured";
    }



    // This function should call the setup of its parent and
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void LitMaterial::setup() const {
        //DONE: (Req 6) Write this function
        TexturedMaterial::setup();

        glActiveTexture(GL_TEXTURE0);
        diffusion->bind();
        sampler->bind(0);
        shader->set("material.diffusion", 0);
        
        glActiveTexture(GL_TEXTURE1);
        specular->bind();
        sampler->bind(1);
        shader->set("material.specular", 1);
        
        glActiveTexture(GL_TEXTURE2);
        ambient_occlusion->bind();
        sampler->bind(2);
        shader->set("material.ambient_occlusion", 2);
        
        glActiveTexture(GL_TEXTURE3);
        roughness->bind();
        sampler->bind(3);
        shader->set("material.roughness", 3);
        
        glActiveTexture(GL_TEXTURE4);
        emissive->bind();
        sampler->bind(4);
        shader->set("material.emissive", 4);

        glActiveTexture(GL_TEXTURE0);


        }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json& data){
        TexturedMaterial::deserialize(data);
        if(!data.is_object()) return;
        diffusion = AssetLoader<Texture2D>::get(data.value("diffusion", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        
    }


    // This function returns the type of material
    std::string LitMaterial::material_type()
    {
        return "lit";
    }



}