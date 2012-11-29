// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef GLOSSYVOLUME_H
#define GLOSSYVOLUME_H

#include <vector>
#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Light.h"
#include "Volume.h"
#include "Phong.h"

class GlossyVolume : public Volume, public Phong
{
public:
  GlossyVolume(RayTracer* raytracer, const std::vector<Light*>& light_vector, int max_trace_depth = 10) 
    : Volume(raytracer, max_trace_depth), Phong(light_vector) 
  { }

  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
};

#endif // GLOSSYVOLUME_H
