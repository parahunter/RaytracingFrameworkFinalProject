// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <vector>
#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Light.h"
#include "Textured.h"

class Lambertian : public Textured
{
public:
  Lambertian(const std::vector<Light*>& light_vector) : lights(light_vector) { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;

protected:
  const std::vector<Light*>& lights;
};

#endif // LAMBERTIAN_H
