#include "SceneOpening.h"
#include <imgui.h>

void SceneOpening::onLoad()
{
	OutputDebugString(L">>>>>>>>>> SceneOpening::onLoad\n");
}

void SceneOpening::onUnload()
{
    OutputDebugString(L">>>>>>>>>> SceneOpening::onUnload\n");
}

void SceneOpening::onUpdate(float dt)
{
}

void SceneOpening::ImGuiMainMenu()
{
    ImGui::Begin("Colour Picker");

    float colour[3] = { _bgColour.x, _bgColour.y, _bgColour.z };

    ImGui::Text("Set background colour:");
    if (ImGui::ColorEdit3("Colour", colour)) 
    {
        DirectX::XMFLOAT4 newColour = { colour[0], colour[1], colour[2], 1.0f };
        //if (newColour.x != _bgColour.x || newColour.y != _bgColour.y || newColour.z != _bgColour.z)
        {
            _bgColour = newColour;
            if (_onColourChange)
                _onColourChange(_bgColour);
        }
    }
   

    ImGui::End();
}
