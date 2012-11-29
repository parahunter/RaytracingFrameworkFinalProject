// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "GlossyVolume.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 GlossyVolume::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Compute the specular part of the glossy shader and attenuate it
  // by the transmittance of the material if the ray is inside (as in
  // the volume shader).

	float3 transmittance = make_float3(1);

	if(dot(r.direction, hit.shading_normal) > 0)
	{
		transmittance = get_transmittance(hit);
	}
		
	float3 rho_s = get_specular(hit);
	float s = get_shininess(hit);

		float3 result = make_float3(0);
  
		for(int i = 0 ; i < lights.size() ; i++)
		{
			float3 L;
			float3 dir;
			if( lights[i]->sample(hit.position, dir, L) )
			{
				float3 R = optix::reflect(-dir,hit.shading_normal);
				unsigned int sInt = (int)s;

				result += ( rho_s*L* int_pow(optix::dot(R, -r.direction), sInt));
			}
		}

		return Volume::shade(r, hit, emit) + result * transmittance;
}
