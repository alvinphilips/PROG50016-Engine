#include "EngineCore.h"
#include "CircleCollider.h"

IMPLEMENT_DYNAMIC_CLASS(CircleCollider);
bool CircleCollider::HandleCollision(ICollider* other)
{
	return true;
}

void CircleCollider::Initialize()
{
	ICollider::Initialize();

	if (m_rect == nullptr) {
		LOG("rect was null");
		return;
	}

	m_radius = Vec2((float)m_rect->w, (float)m_rect->h).Magnitude();
}

/**
 * @brief Sets the radius of the CircleCollider.
 *
 * @param radius The new radius for the collider.
 */
void CircleCollider::SetRadius(float radius)
{
	m_radius = radius;
}

/**
 * @brief Gets the radius of the CircleCollider.
 *
 * @return The radius of the collider.
 */
float CircleCollider::GetRadius()
{
	if (m_radius == 0) {
		m_radius = (Vec2((float)m_rect->w * 0.5f, (float)m_rect->h * 0.5f) * ownerEntity->GetTransform().scale).Magnitude();
	}
	return m_radius;
}
/**
 * @brief Calculates the broad phase radius of the CircleCollider.
 *
 * @return The radius of the collider.
 */
float CircleCollider::GetBroadPhaseRadius() const
{
	return m_radius;
}
/**
 * @brief Gets the type of the collider.
 *
 * @return The type of the collider, in this case Circle.
 */
ColliderType CircleCollider::GetType() const
{
	return ColliderType::Circle;
}