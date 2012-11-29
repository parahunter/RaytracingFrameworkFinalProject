// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef VOLUME_H
#define VOLUME_H

#include <optix_world.h>
#include "HitInfo.h"
#include "RayTracer.h"
#include "Transparent.h"

class Volume : public Transparent
{
public:
  Volume(RayTracer* raytracer, unsigned int max_trace_depth = 20) : Transparent(raytracer, max_trace_depth) { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;

protected:
  optix::float3 get_transmittance(const HitInfo& hit) const;
};

#endif // VOLUME_H
