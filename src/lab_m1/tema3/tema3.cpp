#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}

void Tema3::CreateBalloon(const glm::vec3& position, const std::string& textureName)
{
    // 1. Nacela
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0.0f, -1, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["NormalShader"], modelMatrix, mapTextures["crate"]);
    }

    // 2. Corpul balonului
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0.0f, 0.5f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1f));
        RenderSimpleMesh(meshes["sphere"], shaders["DeformShader"], modelMatrix, mapTextures[textureName]);
    }

    // 3. Conectoarele
    for (int i = 0; i < 4; ++i) {
        glm::mat4 modelMatrix = glm::mat4(1);
        float x = (i % 2 == 0 ? -0.2f : 0.2f);
        float z = (i / 2 == 0 ? -0.2f : 0.2f);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(x, -0.5f, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
        RenderSimpleMesh(meshes["box"], shaders["NormalShader"], modelMatrix, mapTextures["crate"]);
    }
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

    // Seteaza pozitia camerei
    GetSceneCamera()->SetPosition(glm::vec3(0.0f, 5.0f, 12.0f)); // Pozitioneaza camera mai in spate si mai sus
    GetSceneCamera()->Update();
    
    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rainbow.png").c_str(), GL_REPEAT);
        mapTextures["rainbow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "checkerboard.png").c_str(), GL_REPEAT);
        mapTextures["checkerboard"] = texture;
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "jack.png").c_str(), GL_REPEAT);
        mapTextures["jack"] = texture;
    }


    {
        mapTextures["random"] = CreateRandomTexture(55, 55);
    }

    // Pozitii ale baloanelor
    std::vector<glm::vec3> balloonPositions = {
        glm::vec3(0.0f, 1.5f, 0.0f),
        glm::vec3(3.0f, 2.0f, -2.0f),
        glm::vec3(-3.0f, 3.0f, 1.0f),
        glm::vec3(2.0f, 3.5f, 3.0f),
        glm::vec3(-2.0f, 2.5f, -3.0f)
    };

    // Texturi disponibile
    std::vector<std::string> availableTextures = {
        "rainbow",
        "checkerboard",
        "earth",
        "jack",
        "random"
    };

    // Asociaza fiecarui balon o textura unica
    for (const auto& position : balloonPositions) {
        int textureIndex = rand() % availableTextures.size();
        std::string selectedTexture = availableTextures[textureIndex];
        availableTextures.erase(availableTextures.begin() + textureIndex);

        balloons.emplace_back(position, selectedTexture);
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("NormalShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Shader pentru balon
    {
        Shader* deformShader = new Shader("DeformShader");
        deformShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "DeformVertexShader.glsl"), GL_VERTEX_SHADER);
        deformShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "DeformFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        deformShader->CreateAndLink();
        shaders[deformShader->GetName()] = deformShader;
    }

    const glm::vec3 center(0.0f, 0.0f, 0.0f); // Centrul cercurilor
    const std::vector<std::string> availableTexturesBalloons = {
        "rainbow", "checkerboard", "earth", "jack", "random"
    };

    for (size_t i = 0; i < availableTexturesBalloons.size(); ++i) {
        // Genereaza un centru aleatoriu pentru fiecare balon
        glm::vec3 centerOffset = glm::vec3(rand() % 5 - 2, 0.0f, rand() % 5 - 2); // Centrele sunt dispersate pe XOZ
        balloonCenters.push_back(center + centerOffset); // Adauga centrul in vector

        balloonRadii.push_back(2.0f + i);                          // Raze diferite
        balloonHeights.push_back(1.0f + i);                        // Inaltimi diferite
        balloonAngularSpeeds.push_back(5.0f / (1.0f + balloonRadii[i])); // Vitezea unghiulara
        balloonOscillationAmplitudes.push_back(1.2f);              // Amplitudinea oscilatiei
        balloonOscillationFrequencies.push_back(2.0f);             // Frecventa oscilatiei
        balloonTextures.push_back(availableTexturesBalloons[i]);   // Texturi asociate
    }

}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.96, 0.64, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    const float time = Engine::GetElapsedTime(); // Timp global

    for (size_t i = 0; i < balloonCenters.size(); ++i) {
        // Calcularea pozitiei curente pe traiectoria circulara
        float angle = balloonAngularSpeeds[i] * time; // Unghiul curent
        float x = balloonCenters[i].x + balloonRadii[i] * cos(angle);
        float z = balloonCenters[i].z + balloonRadii[i] * sin(angle);

        // Oscilatia pe axa Y
        float oscillation = balloonOscillationAmplitudes[i] * sin(balloonOscillationFrequencies[i] * time);
        float y = balloonHeights[i] + oscillation;

        glm::vec3 currentPosition(x, y, z);

        // Randeaza balonul la pozitia curenta
        CreateBalloon(currentPosition, balloonTextures[i]);
    }

}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
    }

    else
    {
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 0);
        glUniform1i(glGetUniformLocation(shader->program, "flag"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }
    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);


    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
