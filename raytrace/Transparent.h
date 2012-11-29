// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include <optix_world.h>
#include "HitInfo.h"
#include "RayTracer.h"
#include "Mirror.h"

class Transparent : public Mirror
{
public:
  Transparent(RayTracer* raytracer, unsigned int max_trace_depth = 10) 
    : Mirror(raytracer, max_trace_depth)
  { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
};

#endif // TRANSPARENT_H
