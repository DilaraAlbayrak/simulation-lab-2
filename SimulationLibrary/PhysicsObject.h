#pragma once
#include "SJGLoader.h"
#include "Collider.h"
#include <memory>

struct ConstantBuffer
{
    DirectX::XMMATRIX World;
    DirectX::XMFLOAT4 LightColour = { 0.8f, 0.8f, 0.8f, 1.0f };  // White
    DirectX::XMFLOAT4 DarkColour = { 0.2f, 0.2f, 0.2f, 1.0f };  // Black
    DirectX::XMFLOAT2 CheckerboardSize = { 1.0f, 1.0f }; // Adjust tile size
	DirectX::XMFLOAT2 Padding = { 0.0f, 0.0f };
};

enum IntegrationMethod
{
	SEMI_IMPLICIT_EULER,
	RK4
};

class PhysicsObject
{
private:
    std::unique_ptr<Collider> collider;

    std::vector<Vertex> vertices;
    std::vector<int> indices;

	ConstantBuffer constantBuffer;

	// physics-related data
	DirectX::XMFLOAT3 previousPosition = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 angularVelocity = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 acceleration = { 0.0f, 0.0f, 0.0f }; 
    float mass = 1.0f;
    float inverseMass = 1.0f; 
	bool isStatic = false;
	bool applyGravity = !isStatic;
	bool colliding = false;
    std::vector<DirectX::XMFLOAT3> forces;
	DirectX::XMFLOAT3 gravitationalForce = { 0.0f, 0.0f, -9.81f };

	IntegrationMethod integrationMethod = SEMI_IMPLICIT_EULER;

	void moveSemiImplicitEuler(float dt);
	void moveRK4(float dt);

	void updateRotation(float dt)
	{
		DirectX::XMFLOAT3 deltaRot = { angularVelocity.x * dt, angularVelocity.y * dt, angularVelocity.z * dt };
		collider->incrementRotation(deltaRot);
		constantBuffer.World = collider->updateWorldMatrix();
	}

public:
    PhysicsObject(std::unique_ptr<Collider> col, bool fixed = false)
		: collider(std::move(col)), isStatic(fixed)
    {
        inverseMass = (mass != 0.0f) ? 1.0f / mass : 0.0f;
        gravitationalForce = { 0.0f, 0.0f, -9.81f * mass };
		colliding = false;
		constantBuffer.World = collider->updateWorldMatrix();
    }

	bool checkCollision(const PhysicsObject& other) const
	{
		if (!collider || !other.collider) return false;

		return collider->isColliding(*other.collider);
	}

    bool LoadModel(const std::string& filename)
    {
        return SJGLoader::Load(filename, vertices, indices);
    }

    void Update(float deltaTime)
    {
		if (isStatic) return;

		if (applyGravity)
		{
            acceleration = { gravitationalForce.x * inverseMass,
                             gravitationalForce.y * inverseMass,
                             gravitationalForce.z * inverseMass };
		}
		else
		{
			acceleration = { 0.0f, 0.0f, 0.0f };
		}

		if (integrationMethod == SEMI_IMPLICIT_EULER)
		{
            moveSemiImplicitEuler(deltaTime);
		}
		else if (integrationMethod == RK4)
		{
			moveRK4(deltaTime);
		}

		updateRotation(deltaTime);
		constantBuffer.World = collider->updateWorldMatrix();
    }

    void applyForce(const DirectX::XMFLOAT3& force)
    {
        forces.push_back(force);
    }

    const Collider& getCollider() const { return *collider; }
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<int>& getIndices() const { return indices; }
    const DirectX::XMMATRIX& getTransformMatrix() const { return constantBuffer.World; }
	const ConstantBuffer getConstantBuffer() const { return constantBuffer; }
	const bool getStaticInfo() const { return isStatic; }
	const bool getColidingInfo() const { return colliding; }

	void setVelocity(const DirectX::XMFLOAT3& newVel) { velocity = newVel; }
	void setAngularVelocity(const DirectX::XMFLOAT3& newAngVel) { angularVelocity = newAngVel; }
	void setConstantBuffer(const ConstantBuffer& cb) { constantBuffer = cb; }
	void setIntegrationMethod(int method) { integrationMethod = static_cast<IntegrationMethod>(method); }
	void setGravity(bool gravity) { applyGravity = gravity; }
	void setColliding(bool collide) { colliding = collide; }
};