// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <vector>
#include <optix_world.h>
#include "SphereTexture.h"
#include "Tracer.h"

class RayCaster : public Tracer
{
public:
  RayCaster(unsigned int w, unsigned int h, Scene* s, unsigned int pixel_subdivs = 1)
    : Tracer(w, h, s), subdivs(pixel_subdivs), background(optix::make_float3(1.0f)), sphere_tex(0)
  { 
    compute_jitters();
  }
  
  virtual optix::float3 compute_pixel(unsigned int x, unsigned int y) const;

  void set_background(const optix::float3& color) { background = color; }
  void set_background(SphereTexture* sphere_texture) { sphere_tex = sphere_texture; }
  const optix::float3& get_background() const { return background; }
  optix::float3 get_background(const optix::float3& direction) const;
  void increment_pixel_subdivs();
  void decrement_pixel_subdivs();

protected:
  void compute_jitters();

  unsigned int subdivs;
  std::vector<optix::float2> jitter;
  optix::float2 win_to_ip;
  optix::float2 lower_left;
  optix::float2 step;
  optix::float3 background;
  SphereTexture* sphere_tex;
};

#endif // RAYCASTER_H
