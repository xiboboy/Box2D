/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_SHAPE_H
#define B2_SHAPE_H

#include <Box2D/Common/b2BlockAllocator.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/b2Collision.h>

namespace box2d {

/// This holds the mass data computed for a shape.
struct b2MassData
{
	b2MassData() = default;

	constexpr b2MassData(b2Float m, b2Vec2 c, b2Float _I) noexcept: mass(m), center(c), I(_I)
	{
		b2Assert(mass >= 0);
	}

	/// The mass of the shape, usually in kilograms.
	/// This should NEVER be a negative value.
	/// @note Behavior is undefined if this value is negative.
	b2Float mass;

	/// The position of the shape's centroid relative to the shape's origin.
	b2Vec2 center;

	/// The rotational inertia of the shape about the local origin.
	b2Float I;
};

/// A shape is used for collision detection. You can create a shape however you like.
/// Shapes used for simulation in b2World are created automatically when a b2Fixture
/// is created. Shapes may encapsulate a one or more child shapes.
class b2Shape
{
public:
	enum Type
	{
		e_circle = 0,
		e_edge = 1,
		e_polygon = 2,
		e_chain = 3,
		e_typeCount = 4
	};

	b2Shape() = delete;

	/// Initializing constructor.
	/// @param type Type of this shape object.
	/// @param radius Non-negative "radius" distance of this object (whose meaning is
	///   class dependent).
	/// @note Behavior is undefined if a negative radius is given.
	constexpr b2Shape(Type type, b2Float radius) noexcept: m_type(type), m_radius(radius)
	{
		b2Assert(radius >= 0);
	}

	b2Shape(const b2Shape&) = default;

	virtual ~b2Shape() = default;

	/// Clone the concrete shape using the provided allocator.
	virtual b2Shape* Clone(b2BlockAllocator* allocator) const = 0;

	/// Get the type of this shape. You can use this to down cast to the concrete shape.
	/// @return the shape type.
	Type GetType() const noexcept { return m_type; }

	/// Get the number of child primitives.
	virtual child_count_t GetChildCount() const = 0;

	/// Test a point for containment in this shape. This only works for convex shapes.
	/// @param xf the shape world transform.
	/// @param p a point in world coordinates.
	virtual bool TestPoint(const b2Transform& xf, const b2Vec2& p) const = 0;

	/// Cast a ray against a child shape.
	/// @param output the ray-cast results.
	/// @param input the ray-cast input parameters.
	/// @param transform the transform to be applied to the shape.
	/// @param childIndex the child shape index
	virtual bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
						const b2Transform& transform, child_count_t childIndex) const = 0;

	/// Given a transform, compute the associated axis aligned bounding box for a child shape.
	/// @param xf the world transform of the shape.
	/// @param childIndex the child shape
	/// @return the axis aligned box.
	virtual b2AABB ComputeAABB(const b2Transform& xf, child_count_t childIndex) const = 0;

	/// Compute the mass properties of this shape using its dimensions and density.
	/// The inertia tensor is computed about the local origin.
	/// @param massData returns the mass data for this shape.
	/// @param density the density in kilograms per meter squared.
	virtual b2MassData ComputeMass(b2Float density) const = 0;

	/// Gets the "radius" of the shape.
	/// @return a non-negative distance whose meaning is dependent on the object's class.
	b2Float GetRadius() const noexcept { return m_radius; }

	void SetRadius(b2Float radius) noexcept
	{
		b2Assert(radius >= 0);
		m_radius = radius;
	}

private:
	const Type m_type;
	b2Float m_radius;
};

} // namespace box2d

#endif
