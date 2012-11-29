// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Mirror.h"

using namespace optix;

float3 Mirror::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  if(hit.trace_depth >= max_depth)
    return make_float3(0.0f);

  Ray reflected;
  HitInfo reflected_hit;
  tracer->trace_reflected(r, hit, reflected, reflected_hit);
  return shade_new_ray(reflected, reflected_hit);
}

float3 Mirror::shade_new_ray(Ray& r, HitInfo& hit, bool emit) const
{
  if(hit.has_hit)
  {
    const Shader* s = tracer->get_shader(hit);
    if(s)
      return s->shade(r, hit, true);
  }
  else
    return tracer->get_background(r.direction);  

  return make_float3(0.0f);
}
