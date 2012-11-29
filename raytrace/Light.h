// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef LIGHT_H
#define LIGHT_H

#include <optix_world.h>

class RayTracer;

class Light
{
public:
  Light(RayTracer* ray_tracer = 0, unsigned int no_of_samples = 1) 
    : tracer(ray_tracer), samples(no_of_samples), shadows(true) 
  { }

  virtual bool sample(const optix::float3& pos, optix::float3& dir, optix::float3& L) const = 0;
  virtual bool emit(optix::Ray& r, HitInfo& hit, optix::float3& Phi) const { return false; }

  unsigned int get_no_of_samples() const { return samples; }

  void toggle_shadows() { shadows = !shadows; }
  bool generating_shadows() const { return shadows; }

protected:
  bool shadows;
  unsigned int samples;
  RayTracer* tracer;
};

#endif // LIGHT_H
