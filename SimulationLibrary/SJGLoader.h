#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

struct VertexData
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
};

class SJGLoader
{
public:
    static bool Load(const std::string& filename, std::vector<VertexData>& vertices, std::vector<int>& indices);
};