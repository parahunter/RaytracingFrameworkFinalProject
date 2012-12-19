// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Plane.h"

using namespace optix;

bool Plane::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
  // Implement ray-plane intersection here.
  // It is fine to intersect with the front-facing side of the plane only.
  //
  // Input:  r                    (the ray to be checked for intersection)
  //         prim_idx             (index of the primitive element in a collection, not used here)
  //
  // Output: hit.has_hit          (true if the ray intersects the plane, false otherwise)
  //         hit.dist             (distance from the ray origin to the intersection point)
  //         hit.geometric_normal (the normalized normal of the plane)
  //         hit.shading_normal   (the normalized normal of the plane)
  //         hit.material         (pointer to the material of the plane)
  //        (hit.texcoord)        (texture coordinates of intersection point, not needed for Week 1)
  //
  // Return: True if the ray intersects the plane, false otherwise 
  //         (do not return hit.has_hit as it is shared between all primitives and remains true once it is set true)
  //
  // Relevant data fields that are available (see Plane.h and OptiX math library reference)
  // r.origin                     (ray origin)
  // r.direction                  (ray direction)
  // r.tmin                       (minimum intersection distance allowed)
  // r.tmax                       (maximum intersection distance allowed)
  // onb                          (orthonormal basis of the plane: normal, tangent, binormal)
  // material                     (material of the plane)
  //
  // Hint: The OptiX math library has a function dot(v, w) which returns
  //       the dot product of the vectors v and w.

	float t = -( optix::dot(r.origin, get_normal()) + d ) / ( optix::dot(r.direction, get_normal()) );
	
	if(t >= r.tmin && t <= r.tmax)
	{
		hit.has_hit = true;
		hit.position = r.origin + r.direction*t;
		hit.dist = t;
        hit.geometric_normal = get_normal(); // (the normalized normal of the plane)
        hit.shading_normal = get_normal();   // (the normalized normal of the plane)
        hit.material = &material;             // (pointer to the material of the plane)

		if(material.has_texture)
		{	
			float3 vector = hit.position - position;
			
			// to get a basis change matrix we need an inverse matrix of [tangent, binormal, normal]
			// as the vectors in the basis are orthogonal, we can use transposed matrix instead

			float3 transposed1 = make_float3(onb.m_tangent.x, onb.m_binormal.x, onb.m_normal.x);
			float3 transposed2 = make_float3(onb.m_tangent.y, onb.m_binormal.y, onb.m_normal.y);
			float3 transposed3 = make_float3(onb.m_tangent.z, onb.m_binormal.z, onb.m_normal.z);
			
			hit.texcoord = (vector.x * transposed1 + vector.y * transposed2 + vector.z * transposed3) * tex_scale;		
		}

		return true;
	}
	else
	{
		 return false;
	}
}

void Plane::transform(const Matrix4x4& m)
{
  onb = Onb(normalize(make_float3(m*make_float4(onb.m_normal, 0.0f))));
  position = make_float3(m*make_float4(position, 1.0f));
  d = -dot(position, onb.m_normal);
}

Aabb Plane::compute_bbox() const
{
  return Aabb(make_float3(-1e37f), make_float3(1e37f));
}
