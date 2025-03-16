#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "components/simple_scene.h"
#include "components/transform.h"

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
        void CreateBalloon(const glm::vec3& position, const std::string& textureName);
        std::vector<glm::vec3> balloonInitialPositions;
        std::vector<float> balloonRadii;
        std::vector<float> balloonHeights;
        std::vector<float> balloonAngularSpeeds;
        std::vector<float> balloonOscillationAmplitudes;
        std::vector<float> balloonOscillationFrequencies;
        std::vector<std::string> balloonTextures;
        std::vector<glm::vec3> balloonCenters;

    private:

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::vector<std::pair<glm::vec3, std::string>> balloons;
    };
}   // namespace m1
