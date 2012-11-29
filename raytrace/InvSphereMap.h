// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef INVSPHEREMAP_H
#define INVSPHEREMAP_H

#include <optix_world.h>
#include "SphereTexture.h"

class InvSphereMap : public SphereTexture
{
public:
  virtual void project_direction(const optix::float3& d, float& u, float& v) const;
};

#endif // INVSPHEREMAP_H
