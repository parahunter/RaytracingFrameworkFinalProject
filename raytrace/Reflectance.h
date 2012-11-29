// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef REFLECTANCE_H
#define REFLECTANCE_H

#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Emission.h"

class Reflectance : public Emission
{
public:
  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const 
  { 
    return get_diffuse(hit) + Emission::shade(r, hit, emit); 
  }

protected:
  virtual optix::float3 get_diffuse(const HitInfo& hit) const
  {
    const ObjMaterial* m = hit.material;
    return m ? optix::make_float3(m->diffuse[0], m->diffuse[1], m->diffuse[2]) : optix::make_float3(0.8f);
  }
};

#endif // REFLECTANCE_H
