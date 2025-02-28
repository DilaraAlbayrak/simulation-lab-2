#include "pch.h"
#include "PhysicsObject.h"

void PhysicsObject::moveSemiImplicitEuler(float dt)
{
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;
    velocity.z += acceleration.z * dt;

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    position.z += velocity.z * dt;
}

void PhysicsObject::moveRK4(float dt)
{
    DirectX::XMFLOAT3 k1_v = velocity; 
    DirectX::XMFLOAT3 k1_a = acceleration; 

    DirectX::XMFLOAT3 mid_v = {
        velocity.x + 0.5f * dt * k1_a.x,
        velocity.y + 0.5f * dt * k1_a.y,
        velocity.z + 0.5f * dt * k1_a.z
    };
    DirectX::XMFLOAT3 k2_v = mid_v;
    DirectX::XMFLOAT3 k2_a = k1_a;

    mid_v = {
        velocity.x + 0.5f * dt * k2_a.x,
        velocity.y + 0.5f * dt * k2_a.y,
        velocity.z + 0.5f * dt * k2_a.z
    };
    DirectX::XMFLOAT3 k3_v = mid_v;
    DirectX::XMFLOAT3 k3_a = k2_a;

    DirectX::XMFLOAT3 end_v = {
        velocity.x + dt * k3_a.x,
        velocity.y + dt * k3_a.y,
        velocity.z + dt * k3_a.z
    };
    DirectX::XMFLOAT3 k4_v = end_v;
    DirectX::XMFLOAT3 k4_a = k3_a;

    velocity.x += (dt / 6.0f) * (k1_a.x + 2.0f * k2_a.x + 2.0f * k3_a.x + k4_a.x);
    velocity.y += (dt / 6.0f) * (k1_a.y + 2.0f * k2_a.y + 2.0f * k3_a.y + k4_a.y);
    velocity.z += (dt / 6.0f) * (k1_a.z + 2.0f * k2_a.z + 2.0f * k3_a.z + k4_a.z);

    position.x += (dt / 6.0f) * (k1_v.x + 2.0f * k2_v.x + 2.0f * k3_v.x + k4_v.x);
    position.y += (dt / 6.0f) * (k1_v.y + 2.0f * k2_v.y + 2.0f * k3_v.y + k4_v.y);
    position.z += (dt / 6.0f) * (k1_v.z + 2.0f * k2_v.z + 2.0f * k3_v.z + k4_v.z);
}
