// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef EMISSION_H
#define EMISSION_H

#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Shader.h"

class Emission : virtual public Shader
{
public:
  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const 
  { 
    return emit ? get_emission(hit) : optix::make_float3(0.0f); 
  }

protected:
  virtual optix::float3 get_emission(const HitInfo& hit) const
  {
    const ObjMaterial* m = hit.material;
    return m ? optix::make_float3(m->ambient[0], m->ambient[1], m->ambient[2]) : optix::make_float3(0.2f);
  }
};

#endif // EMISSION_H
