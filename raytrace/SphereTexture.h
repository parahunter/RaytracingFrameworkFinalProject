// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef SPHERETEXTURE_H
#define SPHERETEXTURE_H

#include <optix_world.h>
#include "HDRTexture.h"

class SphereTexture : public HDRTexture
{
public:
  virtual optix::float4 sample_nearest(const optix::float3& direction) const;
  virtual optix::float4 sample_linear(const optix::float3& direction) const;
  virtual void project_direction(const optix::float3& d, float& u, float& v) const;
};

#endif // SPHERETEXTURE_H
