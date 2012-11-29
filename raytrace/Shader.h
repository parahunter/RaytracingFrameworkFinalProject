// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef SHADER_H
#define SHADER_H

#include <optix_world.h>
#include "HitInfo.h"

class Shader
{
public:
  virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const = 0;
};

#endif // SHADER_H
