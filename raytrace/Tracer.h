// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef TRACER_H
#define TRACER_H

#include <optix_world.h>
#include "Shader.h"
#include "Scene.h"
#include "HitInfo.h"

class Tracer
{
public:
  Tracer(unsigned int w, unsigned int h, Scene* s)
    : width(w), height(h), scene(s)
  { }

  void set_scene(Scene* s) { scene = s; }
  const Shader* get_shader(const HitInfo& hit) const { return scene ? scene->get_shader(hit) : 0; }
  void get_bsphere(optix::float3& center, float& radius) { if(scene) scene->get_bsphere(center, radius); }

  virtual optix::float3 compute_pixel(unsigned int x, unsigned int y) const = 0;

protected:
  // Resolution
  unsigned int width;
  unsigned int height;

  // The scene
  Scene* scene;
};

#endif // TRACER_H
