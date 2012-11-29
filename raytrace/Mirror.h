// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef MIRROR_H
#define MIRROR_H

#include <optix_world.h>
#include "HitInfo.h"
#include "RayTracer.h"
#include "Shader.h"

class Mirror : virtual public Shader
{
public:
  Mirror(RayTracer* raytracer, unsigned int max_trace_depth = 10) 
    : tracer(raytracer), max_depth(max_trace_depth)
  { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;

protected:
  optix::float3 shade_new_ray(optix::Ray& r, HitInfo& hit, bool emit = true) const;
  
  RayTracer* tracer;
  unsigned int max_depth;
};

#endif // MIRROR_H
