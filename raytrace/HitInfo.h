// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef HITINFO_H
#define HITINFO_H

#include <optix_world.h>
#include "ObjMaterial.h"

class TriMesh;

struct HitInfo
{
  HitInfo() 
    : has_hit(false),
      dist(RT_DEFAULT_MAX),
      trace_depth(0),
      material(0),
      ray_ior(1.0f)
  { }

  bool has_hit;
  float dist;
  optix::float3 position;
  optix::float3 shading_normal;
  optix::float3 geometric_normal;
  optix::float3 texcoord;
  unsigned int trace_depth;
  const ObjMaterial* material;
  float ray_ior;
};

#endif // HITINFO_H