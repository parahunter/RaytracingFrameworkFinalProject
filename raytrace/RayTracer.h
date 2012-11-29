// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <optix_world.h>
#include "Scene.h"
#include "HitInfo.h"
#include "RayCaster.h"

class RayTracer : public RayCaster
{
public:
  RayTracer(unsigned int w, unsigned int h, Scene* s, unsigned int pixel_subdivs = 1)
    : RayCaster(w, h, s, pixel_subdivs)
  { }

  bool trace_to_closest(optix::Ray& r, HitInfo& hit) const { return scene->closest_hit(r, hit); }
  bool trace_to_any(optix::Ray& r, HitInfo& hit) const { return scene->any_hit(r, hit); }
  bool trace_reflected(const optix::Ray& in, const HitInfo& in_hit, optix::Ray& out, HitInfo& out_hit) const;
  bool trace_refracted(const optix::Ray& in, const HitInfo& in_hit, optix::Ray& out, HitInfo& out_hit) const;
  bool trace_refracted(const optix::Ray& in, const HitInfo& in_hit, optix::Ray& out, HitInfo& out_hit, float& fresnel_R) const;

private:
  float get_ior_out(const optix::Ray& in, const HitInfo& in_hit, optix::float3& dir, optix::float3& normal, float& cos_theta_in) const;
};

#endif // RAYTRACER_H
