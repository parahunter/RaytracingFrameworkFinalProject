// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "Glossy.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 Glossy::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Implement glossy reflection here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see Mirror.h and HitInfo.h):
  // max_depth          (maximum trace depth)
  // tracer             (pointer to ray tracer)
  // hit.trace_depth    (number of surface interactions previously suffered by the ray)
  //
  // Hint: Use the function shade_new_ray(...) to pass a newly traced ray to
  //       the shader for the surface it hit.
    
  if(hit.trace_depth >= max_depth)
    return make_float3(0.0f);
  
  /*
  float R;
  Ray reflected, refracted;
  HitInfo hit_reflected, hit_refracted;
  *
  //return Transparent::shade(r, hit, emit);
  
  tracer->trace_reflected(r, hit, reflected, hit_reflected);
  tracer->trace_refracted(r, hit, refracted, hit_refracted, R);

  //return Mirror::shade(r, hit, emit);
  //return Transparent::shade(r, hit, emit);
  //return Phong::shade(r, hit, emit);
  
  return 0.1f*Phong::shade(r, hit, emit) + 
	     0.1f*shade_new_ray(reflected, hit_reflected) + 
		 0.9f* shade_new_ray(refracted, hit_refracted);
  
  //return Phong::shade(r, hit, emit);
}
