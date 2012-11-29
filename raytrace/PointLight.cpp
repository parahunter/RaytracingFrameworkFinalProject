// 02562 Rend'ering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "mt_random.h"
#include "PointLight.h"

using namespace optix;

bool PointLight::sample(const float3& pos, float3& dir, float3& L) const
{
	// Compute output and return value given the following information.
	//
	// Input:  pos (the position of the geometry in the scene)
	//
	// Output: dir (the direction toward the light)
	//         L   (the radiance received from the direction dir)
	//
	// Return: true if not in shadow
	//
	// Relevant data fields that are available (see Directional.h and Light.h):
	// shadows    (on/off flag for shadows)
	// tracer     (pointer to ray tracer)
	// light_pos  (position of the point light)
	// intensity  (intensity of the emitted light)
	//
	// Hint: Construct a shadow ray using the Ray datatype. Trace it using the
	//       pointer to the ray tracer.	
	if(!shadows)
	{
		float3 lightVec = light_pos - pos;
		float length =  optix::length(lightVec);

		dir = lightVec / length;
		L = intensity / length*length;

		return true;
	}
	else
	{
		optix::float3 shadowDir = optix::normalize( light_pos - pos );
		HitInfo hit;

		optix::Ray shadowRay = Ray(pos, shadowDir, 0, 0.0001f);

		if(!tracer->trace_to_any(shadowRay, hit))
		{

			dir = optix::normalize(light_pos - pos);

			L = intensity / ( optix::length(light_pos - pos) * optix::length(light_pos - pos) ) ;

			return true;
		}

		else
		{
			L = make_float3(0,0,0);

			return false;
		}
	}
}

bool PointLight::emit(Ray& r, HitInfo& hit, float3& Phi) const
{
  // Sample ray direction and create ray
	float x,y,z;
	do
	{
		x = randomizer.mt_random() * 2 - 1;
		y = randomizer.mt_random() * 2 - 1;
		z = randomizer.mt_random() * 2 - 1;
	}while(x*x + y*y + z*z > 1);

	float3 dir = normalize(make_float3(x, y, z));
	
  // Trace ray
	r.direction = dir;
	r.origin = light_pos;
	r.tmin = 1e-5;
	r.tmax = 9999;

	if (tracer -> trace_to_closest(r, hit))
	{
		Phi = intensity * 4 * M_PIf;
		return true;
	}

  // If a surface was hit, compute Phi and return true

  return false;
}
