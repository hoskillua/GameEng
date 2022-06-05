#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include <iostream>
namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //DONE: (Req 9) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            skyPipelineState.faceCulling.frontFace = GL_CCW;
            

            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }


        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            defaultPostprocess = config.value<std::string>("postprocess", "");
            updatePostprocess(defaultPostprocess);
        }
    }
    void ForwardRenderer::updatePostprocess(std::string postprocess){
        
        if(postprocess == ""){
            postprocess = defaultPostprocess;
        }
        //DONE: (Req 10) Create a framebuffer
        glGenFramebuffers(1, &postprocessFrameBuffer);
        //DONE: (Req 10) Create a color and a depth texture and attach them to the framebuffer
        // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
        // The depth format can be (Depth component with 24 bits).
        // The size of the textures should be the same as the window size.
        colorTarget = new Texture2D();
        depthTarget = new Texture2D();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
        glBindTexture(GL_TEXTURE_2D, colorTarget->getOpenGLName());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, depthTarget->getOpenGLName());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

        //DONE: (Req 10) Unbind the framebuffer just to be safe
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // Create a vertex array to use for drawing the texture
        glGenVertexArrays(1, &postProcessVertexArray);

        // Create a sampler to use for sampling the scene texture in the post processing shader
        Sampler* postprocessSampler = new Sampler();
        postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Create the post processing shader
        ShaderProgram* postprocessShader = new ShaderProgram();
        postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
        postprocessShader->attach(postprocess, GL_FRAGMENT_SHADER);
        postprocessShader->link();

        // Create a post processing material
        postprocessMaterial = new TexturedMaterial();
        postprocessMaterial->shader = postprocessShader;
        postprocessMaterial->texture = colorTarget;
        postprocessMaterial->sampler = postprocessSampler;
        // The default options are fine but we don't need to interact with the depth buffer
        // so it is more performant to disable the depth mask
        postprocessMaterial->pipelineState.depthMask = false;
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;

        // Define a vector for all light components
        std::vector<LightComponent*> lights;

        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            
            // If this entity has light component
            if(auto lighter = entity->getComponent<LightComponent>(); lighter)
            {
                    lights.push_back(lighter);
            }
            
            
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //DONE: (Req 8) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        glm::vec3 cameraForward = glm::vec3(camera->getViewMatrix() * glm::vec4(0, 0, -1, 0)); 
        //glm::vec3 cameraForward = glm::vec3(0.0, 0.0, 0.0);
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //DONE: (Req 8) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            // The camera forward vector is given as a parameter to this function.
            // You can use the dot product to compute the distance between the camera and the object.
            // The closer the object is, the higher the returned value.
            // The farther the object is, the lower the returned value.
            // If you get an error here, you probably forgot to implement the function        
             float first_dis = glm::dot(cameraForward, first.center);
            float second_dis = glm::dot(cameraForward, second.center);
             
            return first_dis>second_dis;
        });

        //DONE: (Req 8) Get the camera ViewProjection matrix and store it in VP
        glm::mat4 VP = camera->getProjectionMatrix(windowSize)* camera->getViewMatrix();
        //DONE: (Req 8) Set the OpenGL viewport using windowSize
        glViewport(0, // x
                   0, // y
                   windowSize.x, // width
                   windowSize.y); // height
      
        //DONE: (Req 8) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        //DONE: (Req 8) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //DONE: (Req 10) bind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

        }

        //DONE: (Req 8) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //DONE: (Req 8) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (int i = 0; i < opaqueCommands.size(); i++)
        {
            //setup the material of the command
            opaqueCommands[i].material->setup();

            //Set the uniform transform
            opaqueCommands[i].material->shader->set("transform", VP * opaqueCommands[i].localToWorld);


            //Set the uniform VP (useful to get the normal with lighning)
            opaqueCommands[i].material->shader->set("VP", VP);

            //Set the uniform M (useful to get the normal with lighning)
            opaqueCommands[i].material->shader->set("M", opaqueCommands[i].localToWorld);

            //Set the uniform M_Inverse (useful to get the normal with lighning)
            opaqueCommands[i].material->shader->set("M_IT", glm::transpose( glm::inverse( opaqueCommands[i].localToWorld)));

            //Set the uniform is_opaque (useful to get the normal with lighning)
            opaqueCommands[i].material->shader->set("is_opaque", true);

            opaqueCommands[i].material->shader->set("eye",cameraForward );

            if (opaqueCommands[i].material->material_type() == "lit")
            {
            
                //if it is lit material, send data of the light component
                opaqueCommands[i].material->shader->set("light_count" , glm::int16( lights.size()) +1 );


                //Add all lights 
                for (int j = 0; j < lights.size(); j++)
                {
                            
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].type" , lights[j]->type);
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].position" ,  lights[j]->getOwner()->localTransform.position );
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].direction" , lights[j]->getOwner()->localTransform.rotation);
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].diffusion" , lights[j]->diffusion);
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].specular" , lights[j]->specular);
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].attenuation" , lights[j]->attenuation);
                    opaqueCommands[i].material->shader->set("lights[" + std::to_string(j) + "].cone_angles" , glm::vec2(glm::radians(90.0f), glm::radians(90.0f)));




                }                


                opaqueCommands[i].material->shader->set("sky.top" , glm::vec3(1,1,1.0));
                opaqueCommands[i].material->shader->set("sky.middle" , glm::vec3(0.5,0.4,0.5));
                opaqueCommands[i].material->shader->set("sky.bottom" , glm::vec3(0.1,0.1,0));

                opaqueCommands[i].material->shader->set("lights[1].type", 1);
                opaqueCommands[i].material->shader->set( "lights[1].position", glm::vec3( 0, 1, 0));
                opaqueCommands[i].material->shader->set( "lights[1].direction",glm::vec3( 1, 0, 0));
                opaqueCommands[i].material->shader->set( "lights[1].diffusion",glm::vec3( 1, 0, 0));
                opaqueCommands[i].material->shader->set( "lights[1].specular", glm::vec3(1, 0, 0));
                opaqueCommands[i].material->shader->set( "lights[1].attenuation", glm::vec3(0.0, 0.05, 0.5));
                opaqueCommands[i].material->shader->set( "lights[1].cone_angles", glm::vec2(glm::radians(10.0f), glm::radians(20.0f)));



            }


            //Draw Mesh
            opaqueCommands[i].mesh->draw();
        }
        
        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //DONE: (Req 9) setup the sky material
            skyMaterial->setup();
            
            //DONE: (Req 9) Get the camera position
            glm::vec3 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);

            //DONE: (Req 9) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 skyModelMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);

            //DONE: (Req 9) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
           glm::mat4 alwaysBehindTransform = glm::mat4(
            //  Row1, Row2, Row3, Row4
                1.0f, 0.0f, 0.0f, 0.0f, // Column1
                0.0f, 1.0f, 0.0f, 0.0f, // Column2
                0.0f, 0.0f, 0.0f, 0.0f, // Column3
                0.0f, 0.0f, 1.0f, 1.0f  // Column4
            );
            //DONE: (Req 9) set the "transform" uniform
            skyMaterial->shader->set("transform",  alwaysBehindTransform* VP * skyModelMatrix );
            
            //DONE: (Req 9) draw the sky sphere
            skySphere->draw();
            
        }
        //DONE: (Req 8) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (int i = 0; i < transparentCommands.size(); i++)
        {
            //setup the material of the command
            transparentCommands[i].material->setup();

            //Set the uniform transform
            transparentCommands[i].material->shader->set("transform", VP * transparentCommands[i].localToWorld);

            //Draw Mesh
            transparentCommands[i].mesh->draw();
        }
        

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //DONE: (Req 10) Return to the default framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            //DONE: (Req 10) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            //postprocessMaterial->shader->set("transform", glm::mat4(1.0f));
            glBindVertexArray(postProcessVertexArray);
            glDrawArrays(GL_TRIANGLES,0,3);
            
        }
    }

}