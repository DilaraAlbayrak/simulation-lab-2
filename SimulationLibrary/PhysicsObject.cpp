#include "pch.h"
#include "PhysicsObject.h"

void PhysicsObject::updateSemiImplicitEuler(float dt)
{
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;
    velocity.z += acceleration.z * dt;

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    position.z += velocity.z * dt;
}

void PhysicsObject::updateVerlet(float dt)
{
    static DirectX::XMFLOAT3 previousPosition = position;

    // Verlet Position Update: x_new = 2*x - x_prev + a*dt^2
    DirectX::XMFLOAT3 newPosition;
    newPosition.x = 2 * position.x - previousPosition.x + acceleration.x * dt * dt;
    newPosition.y = 2 * position.y - previousPosition.y + acceleration.y * dt * dt;
    newPosition.z = 2 * position.z - previousPosition.z + acceleration.z * dt * dt;

    previousPosition = position;
    position = newPosition;
}