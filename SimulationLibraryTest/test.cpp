#include "pch.h"
#include "SJGLoader.h"

TEST(SJGLoaderTest, CanLoadSJGFile)
{
    std::vector<VertexData> vertices;
    std::vector<int> indices;
    bool result = SJGLoader::Load("sphere.sjg", vertices, indices);

    EXPECT_TRUE(result);
    EXPECT_GT(vertices.size(), 0);
    EXPECT_GT(indices.size(), 0);
}