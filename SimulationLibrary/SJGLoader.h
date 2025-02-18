#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
};

class SJGLoader
{
public:
    static bool Load(const std::string& filename, std::vector<Vertex>& vertices, std::vector<int>& indices);
};