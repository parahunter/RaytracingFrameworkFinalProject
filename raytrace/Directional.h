// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include <string>
#include <optix_world.h>
#include "RayTracer.h"
#include "Light.h"

class Directional : public Light
{
public:
  Directional(RayTracer* ray_tracer, const optix::float3& emitted_radiance, const optix::float3& light_direction) 
    : Light(ray_tracer), emission(emitted_radiance), light_dir(normalize(light_direction)) 
  { }

  virtual bool sample(const optix::float3& pos, optix::float3& dir, optix::float3& L) const;

  std::string describe() const;

protected:
  optix::float3 light_dir;
  optix::float3 emission;
};

#endif // DIRECTIONAL_H
