// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "Phong.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 Phong::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  float3 rho_d = get_diffuse(hit);
  float3 rho_s = get_specular(hit);
  float s = get_shininess(hit);

  // Implement Phong reflection here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see Lambertian.h, Ray.h, and above):
  // lights             (vector of pointers to the lights in the scene)
  // hit.position       (position where the ray hit the material)
  // hit.shading_normal (surface normal where the ray hit the material)
  // rho_d              (difuse reflectance of the material)
  // rho_s              (specular reflectance of the material)
  // s                  (shininess or Phong exponent of the material)
  //
  // Hint: (a) Call the sample function associated with each light in the scene.
  //       (b) I suggest that you convert shininess to an integer and use the
  //       function int_pow(...) whichr is much more efficient than the general
  //       pow(...) function.

  float3 result = make_float3(0);
  
  for(int i = 0 ; i < lights.size() ; i++)
  {
	  float3 L;
	  float3 dir;
	  if( lights[i]->sample(hit.position, dir, L) )
	  {
		  float3 R = optix::reflect(-dir,hit.shading_normal);
		  unsigned int sInt = (int)s;
		  
		  result += rho_d*L*optix::dot(hit.shading_normal, dir) + rho_s*L* int_pow(optix::dot(R, -r.direction), sInt);

	  }

	  
	//return Lambertian::shade(r, hit, emit);
  }

  return result;
}
