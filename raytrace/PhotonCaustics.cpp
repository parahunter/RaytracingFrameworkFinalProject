// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "PhotonCaustics.h"

using namespace optix;

float3 PhotonCaustics::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  float3 rho_d = get_diffuse(hit);

  // Make a radiance estimate using the caustics photon map
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see PhotonCaustics.h and above):
  // rho_d              (difuse reflectance of the material)
  // tracer             (pointer to particle tracer)
  // max_dist           (maximum radius of radiance estimate)
  // photons            (maximum number of photons to be included in radiance estimate)
  //
  // Hint: Use the function tracer->caustics_irradiance(...) to do an
  //       irradiance estimate using the photon map. This is not the 
  //       same as a radiance estimate.
  
  float3 irradiance = tracer -> caustics_irradiance(hit, max_dist, photons);
  
  return irradiance * rho_d + Lambertian :: shade(r, hit, emit);
}
