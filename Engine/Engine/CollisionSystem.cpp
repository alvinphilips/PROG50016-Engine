#include "EngineCore.h"
#include "CollisionSystem.h"

#define NDEBUG_COLLISION_SYSTEM

CollisionSystem* CollisionSystem::instance = nullptr;
/**
 * @brief Initializes the CollisionSystem. Currently not implemented.
 */
void CollisionSystem::Initialize()
{

}
/**
 * @brief Destroys the CollisionSystem. Currently not implemented.
 */
void CollisionSystem::Destroy()
{

}
/**
 * @brief Updates the CollisionSystem, handling collision detection and resolution.
 */
void CollisionSystem::Update()
{
	auto potentialCollisions = BroadPhaseDetection();
	auto currentFrameCollisions = NarrowPhaseDetection(potentialCollisions);

	std::set<std::pair<ICollider*, ICollider*>> collisionsToRemove;

	//flush exit and stay lists
	enterCollisions.clear();
	exitCollisions.clear();

	// Handle new and ongoing collisions
	for (const auto& collisionPair : currentFrameCollisions)
	{
		if (collisionPair.first->IsSolid() && collisionPair.second->IsSolid())
		{
			ResolveCollision(collisionPair.first, collisionPair.second);
		}
		else 
		{
			if (ongoingCollisions.find(collisionPair) == ongoingCollisions.end()) 
			{
				// New collision
				enterCollisions.push_back(collisionPair);
			}
			else 
			{
				// Ongoing collision
				bool inStay = false;
				for (const auto& existingPair : stayCollisions)
				{
					inStay = true;
				}
				if (inStay) { stayCollisions.push_back(collisionPair); }
			}
		}
	}

	// Determine which collisions have ended
	for (const auto& oldCollision : ongoingCollisions) 
	{
		if (currentFrameCollisions.find(oldCollision) == currentFrameCollisions.end()) 
		{
			collisionsToRemove.insert(oldCollision);
			exitCollisions.push_back(oldCollision);
			//flush this pair from the stay list
			bool inList = std::find(stayCollisions.begin(), stayCollisions.end(), oldCollision) != stayCollisions.end();
			if (inList) { stayCollisions.remove(oldCollision); }
		}
	}

	// Remove ended collisions from ongoing collisions
	for (const auto& collisionToRemove : collisionsToRemove) 
	{
		ongoingCollisions.erase(collisionToRemove);
	}

	// Update ongoing collisions for the next frame
	ongoingCollisions = std::move(currentFrameCollisions);
#ifdef DEBUG_COLLISION_SYSTEM
	LOG(ongoingCollisions.size() << " collisions Collisioning.");
#endif
}
/**
 * @brief Adds a collider to the CollisionSystem for tracking.
 *
 * @param collider The collider to be added.
 */
void CollisionSystem::AddCollider(ICollider* collider)
{
	colliders.push_back(collider);
}
/**
 * @brief Removes a collider from the CollisionSystem.
 *
 * @param collider The collider to be removed.
 */
void CollisionSystem::RemoveCollider(ICollider* collider)
{
	colliders.remove(collider);
}
/**
 * @brief Performs broad phase collision detection to identify potential collisions.
 *
 * @return A list of pairs of potentially colliding colliders.
 */
std::list<std::pair<ICollider*, ICollider*>> CollisionSystem::BroadPhaseDetection()
{

	std::list<std::pair<ICollider*, ICollider*>> potentialCollisions;
	for (auto it1 = colliders.begin(); it1 != colliders.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != colliders.end(); ++it2) {

			ICollider* collider1 = *it1;
			ICollider* collider2 = *it2;


			// Calculate bounding circle for each collider, treating them all as circles for broad phase.
			float radius1 = collider1->GetBroadPhaseRadius();
			float radius2 = collider2->GetBroadPhaseRadius();

			Vec2 position1 = collider1->GetPosition();
			Vec2 position2 = collider2->GetPosition();

			Vec2 positionDiff = position1 - position2;
			float radiusSum = radius1 + radius2;
			if(positionDiff.MagnitudeSquared()<= (radiusSum * radiusSum))
			{
				potentialCollisions.push_back(std::make_pair(collider1, collider2));
			}
		}
	}

	return potentialCollisions;
}

/**
 * @brief Performs narrow phase collision detection on potential collisions identified in the broad phase.
 *
 * @param potentialCollisions List of potential colliding pairs from the broad phase.
 * @return A set of pairs of colliders that are actually colliding.
 */
std::set<std::pair<ICollider*, ICollider*>> CollisionSystem::NarrowPhaseDetection(const std::list<std::pair<ICollider*, ICollider*>>& potentialCollisions) {
	
	std::set<std::pair<ICollider*, ICollider*>> currentFrameCollisions;

	for (const auto& collisionPair : potentialCollisions) {
		ICollider* collider1 = collisionPair.first;
		ICollider* collider2 = collisionPair.second;

		bool isCollision = false;

		// Circle vs Circle Collision Check
		if (collider1->GetType() == ColliderType::Circle && collider2->GetType() == ColliderType::Circle) 
		{
			isCollision = CircleCircleCollision((CircleCollider*)collider1, (CircleCollider*)collider2);
		}
		// Box vs Box Collision Check (to be implemented)
		else if (collider1->GetType() == ColliderType::Box && collider2->GetType() == ColliderType::Box)
		{
			isCollision = BoxBoxCollision((BoxCollider*)collider1, (BoxCollider*)collider2);
		}
		// Circle vs Box Collision Check (to be implemented)
		else if (collider1->GetType() == ColliderType::Circle && collider2->GetType() == ColliderType::Box) 
		{
			isCollision = CircleBoxCollision(collider1, (BoxCollider*)collider2);
		}
		// Box vs Circle Collision Check (to be implemented)
		else if (collider1->GetType() == ColliderType::Box && collider2->GetType() == ColliderType::Circle) 
		{
			isCollision = CircleBoxCollision(collider2, (BoxCollider*)collider1);
		}

		// Handle collision
		if (isCollision)
		{
			currentFrameCollisions.insert(collisionPair);
		}
	}
	return currentFrameCollisions;
}

// Helper function for Circle-Circle collision
bool CollisionSystem::CircleCircleCollision(ICollider* col1, ICollider* col2) {
	CircleCollider* circle1 = (CircleCollider*)col1;
	CircleCollider* circle2 = (CircleCollider*)col2;
	Vec2 positionDiff = circle1->GetPosition() - circle2->GetPosition();
	float radiusSum = circle1->GetRadius() + circle2->GetRadius();
	return positionDiff.MagnitudeSquared() <= (radiusSum * radiusSum);
}

// Helper function for Box-Box collision using AABB (Axis-Aligned Bounding Box)
bool CollisionSystem::BoxBoxCollision(ICollider* col1, ICollider* col2) {
	BoxCollider* box1 = (BoxCollider*)col1;
	BoxCollider* box2 = (BoxCollider*)col2;
	auto bounds1 = box1->GetBounds();
	auto bounds2 = box2->GetBounds();
	bounds1.w *= box1->ownerEntity->GetTransform().scale.x;
	bounds1.h *= box1->ownerEntity->GetTransform().scale.y;
	bounds2.w *= box2->ownerEntity->GetTransform().scale.x;
	bounds2.h *= box2->ownerEntity->GetTransform().scale.y;

	// Calculate the half-widths and half-heights
	float halfWidth1 = bounds1.w / 2.0f;
	float halfHeight1 = bounds1.h / 2.0f;
	float halfWidth2 = bounds2.w / 2.0f;
	float halfHeight2 = bounds2.h / 2.0f;

	// Check for overlap in the x axis
	bool overlapX = abs(bounds1.x - bounds2.x) < (halfWidth1 + halfWidth2);
	// Check for overlap in the y axis
	bool overlapY = abs(bounds1.y - bounds2.y) < (halfHeight1 + halfHeight2);

	// Collision occurs if both the x and y axes overlap
	return overlapX && overlapY;
}

// Helper function for Circle-Box collision using AABB (Axis-Aligned Bounding Box)
bool CollisionSystem::CircleBoxCollision(ICollider* col1, ICollider* col2) {
	BoxCollider* box = (BoxCollider*)(col2);
	CircleCollider* circle = (CircleCollider*)(col1);
	auto bounds = box->GetBounds();
	Vec2 circleCenter = circle->GetPosition();
	float circleRadius = circle->GetRadius();

	//Find the closest point on the AABB to the circle center
	float closestX = std::max(float(bounds.x), std::min(circleCenter.x, float(bounds.x + bounds.w)));
	float closestY = std::max(float(bounds.y), std::min(circleCenter.y, float(bounds.y + bounds.h)));

	//Calculate the distance between the circle's center and this closest point
	Vec2 closestPoint(closestX, closestY);
	float distanceSquared = (circleCenter - closestPoint).MagnitudeSquared();

	//If the distance is less than the circle's radius, an intersection occurs
	return distanceSquared < (circleRadius * circleRadius);
}


/**
 * @brief Resolves collision between two solid colliders by resetting their positions.
 *
 * @param col1 First collider involved in the collision.
 * @param col2 Second collider involved in the collision.
 */
void CollisionSystem::ResolveCollision(ICollider* col1, ICollider* col2)
{
}