// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef PHONG_H
#define PHONG_H

#include <vector>
#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Light.h"
#include "Lambertian.h"

class Phong : public Lambertian
{
public:
  Phong(const std::vector<Light*>& light_vector) : Lambertian(light_vector) { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;

protected:
  optix::float3 get_specular(const HitInfo& hit) const
  {
    const ObjMaterial* m = hit.material;
    return m ? optix::make_float3(m->specular[0], m->specular[1], m->specular[2]) : optix::make_float3(0.0f);
  }

  float get_shininess(const HitInfo& hit) const
  {
    const ObjMaterial* m = hit.material;
    return m ? m->shininess : 0.0f;
  }
};

#endif // PHONG_H
