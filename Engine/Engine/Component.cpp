// @file: Component.cpp
//
// @brief: Implementation of the Component class, serving as the base for all components attachable to entities in the game engine.
//

#include "EngineCore.h"
#include "Component.h"
#include "Entity.h"

#define NDEBUG_COMPONENT

IMPLEMENT_ABSTRACT_CLASS(Component)


/**
 * @brief Initializes the component.
 * This function is called during the initialization of the component.
 * Currently, it has an empty implementation but can be overridden in derived classes.
 */
void Component::Initialize()
{
    if (isActive) {
        OnEnable();
    }
}


/**
 * @brief Loads component data from a JSON object.
 *
 * @param node The JSON object containing the component data.
 * In debug mode, logs the loaded JSON data.
 */
void Component::Load(json::JSON& node)
{
    Object::Load(node);
#ifdef DEBUG_COMPONENT
    LOG(node.dump())
#endif
}


/**
 * @brief Destroys the component.
 * This function is called when the component is being destroyed.
 * It sets the ownerEntity pointer to nullptr and calls the base class Destroy function.
 */
void Component::Destroy()
{
    ownerEntity = nullptr;
    Object::Destroy();
}


/**
 * @brief Sets the owner entity of this component.
 *
 * @param owner Pointer to the Entity that owns this component.
 */
void Component::SetOwner(Entity* owner)
{
    ownerEntity = owner;
}


/**
 * @brief Gets the owner entity of this component.
 *
 * @return Pointer to the owner Entity.
 */
Entity* Component::GetOwner() const
{
    return ownerEntity;
}