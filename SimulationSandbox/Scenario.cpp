#include "Scenario.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

void Scenario::initObjects(const std::wstring& shaderFile)
{
    vertexBuffers.clear();
    indexBuffers.clear();
    inputLayouts.clear();
    indexCounts.clear();

    // **Compile the vertex shader for this scene**
    HRESULT hr = ShaderManager::getInstance(device)->compileShaderFromFile(shaderFile.c_str(), "VS", "vs_5_0", &vertexShaderBlob);
    // **Create the Vertex Shader**
    hr = device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr, &vertexShader);

    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create vertex shader\n");
        return;
    }

    hr = ShaderManager::getInstance(device)->compileShaderFromFile(shaderFile.c_str(), "PS", "ps_5_0", &pixelShaderBlob);
    // **Create the Pixel Shader**
    hr = device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr, &pixelShader);

    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create pixel shader\n");
        return;
    }

    for (auto& obj : physicsObjects)
    {
        const std::vector<Vertex>& vertices = obj->getVertices();
        const std::vector<int>& indices = obj->getIndices();

        if (vertices.empty() || indices.empty()) continue;

        // **Define input layout for this object**
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        CComPtr<ID3D11InputLayout> inputLayout;
        hr = device->CreateInputLayout(
            layout, ARRAYSIZE(layout),
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            &inputLayout
        );

        if (FAILED(hr))
        {
            OutputDebugString(L"Failed to create input layout\n");
            continue;
        }

        inputLayouts.push_back(inputLayout);

        // **Create vertex buffer**
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexInitData = {};
        vertexInitData.pSysMem = vertices.data();

        CComPtr<ID3D11Buffer> vertexBuffer;
        hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &vertexBuffer);
        if (FAILED(hr)) continue;

        vertexBuffers.push_back(vertexBuffer);

        // **Create index buffer**
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(int) * indices.size());
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexInitData = {};
        indexInitData.pSysMem = indices.data();

        CComPtr<ID3D11Buffer> indexBuffer;
        hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, &indexBuffer);
        if (FAILED(hr)) continue;

        indexBuffers.push_back(indexBuffer);
        indexCounts.push_back(static_cast<UINT>(indices.size()));

        D3D11_BUFFER_DESC cbDesc = {};
        cbDesc.Usage = D3D11_USAGE_DEFAULT;
        cbDesc.ByteWidth = sizeof(ConstantBuffer);
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        CComPtr <ID3D11Buffer> constantBuffer;
        device->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
        constantBuffers.push_back(constantBuffer);

        // Set primitive topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}

void Scenario::unloadScenario()
{
    physicsObjects.clear();
    vertexBuffers.clear();
    indexBuffers.clear();
    indexCounts.clear();
}

void Scenario::applySharedGUI()
{
    ImGui::Begin("Scenario Controls");
    ImGui::PushItemWidth(100);
    ImGui::Checkbox("Apply Gravity", &applyGravity);
    //ImGui::Text("Integration Method:");
    if (ImGui::RadioButton("Semi-Implicit Euler", integrationMethod == 0)) {
        integrationMethod = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Verlet", integrationMethod == 1)) {
        integrationMethod = 1;
    }
    ImGui::PopItemWidth();
    ImGui::End();
}

void Scenario::renderObjects()
{
    if (!vertexShader || !pixelShader) return;  // Ensure shaders are valid

    for (size_t i = 0; i < physicsObjects.size(); ++i)
    {
        if (!vertexBuffers[i] || !indexBuffers[i] || !inputLayouts[i]) continue;

        // **Set shaders before drawing**
        context->VSSetShader(vertexShader.p, nullptr, 0);
        context->PSSetShader(pixelShader.p, nullptr, 0);

        // **Set the input layout**
        context->IASetInputLayout(inputLayouts[i]);

        // **Set vertex buffer**
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        ID3D11Buffer* vBuffer = vertexBuffers[i].p;
        context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

        // **Set index buffer**
        context->IASetIndexBuffer(indexBuffers[i], DXGI_FORMAT_R32_UINT, 0);

        ConstantBuffer cb = physicsObjects[i]->getConstantBuffer();

        context->UpdateSubresource(constantBuffers[i].p, 0, nullptr, &cb, 0, 0);
        context->VSSetConstantBuffers(1, 1, &constantBuffers[i].p); // register b1
        context->PSSetConstantBuffers(1, 1, &constantBuffers[i].p); // register b1

        // **Draw the object**
        context->DrawIndexed(indexCounts[i], 0, 0);
    }
}
