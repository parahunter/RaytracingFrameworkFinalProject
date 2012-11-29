// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <optix_world.h>
#include "RayTracer.h"
#include "Light.h"

class PointLight : public Light
{
public:
  PointLight(RayTracer* ray_tracer, const optix::float3& emitted_intensity, const optix::float3& position) 
    : Light(ray_tracer), intensity(emitted_intensity), light_pos(position)
  { }

  virtual bool sample(const optix::float3& pos, optix::float3& dir, optix::float3& L) const;
  virtual bool emit(optix::Ray& r, HitInfo& hit, optix::float3& Phi) const;

protected:
  optix::float3 light_pos;
  optix::float3 intensity;
};

#endif // POINTLIGHT_H
