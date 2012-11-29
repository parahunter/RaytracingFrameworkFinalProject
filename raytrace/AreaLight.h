// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef AREALIGHT_H
#define AREALIGHT_H

#include <optix_world.h>
#include "TriMesh.h"
#include "RayTracer.h"
#include "HitInfo.h"
#include "Light.h"

class AreaLight : public Light
{
public:
  AreaLight(RayTracer* ray_tracer, const TriMesh* triangle_mesh, unsigned int no_of_samples = 1) 
    : Light(ray_tracer, no_of_samples), mesh(triangle_mesh)
  { }

  virtual bool sample(const optix::float3& pos, optix::float3& dir, optix::float3& L) const;
  virtual bool emit(optix::Ray& r, HitInfo& hit, optix::float3& Phi) const;

protected:
  optix::float3 get_emission(unsigned int triangle_id) const;

  const TriMesh* mesh;
};

#endif // AREALIGHT_H
