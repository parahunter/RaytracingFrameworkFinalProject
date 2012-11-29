// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <optix_world.h>
#include "HitInfo.h"

class Object3D
{
public:
  virtual bool intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const = 0;
  virtual void transform(const optix::Matrix4x4& m) = 0;
  virtual optix::Aabb compute_bbox() const = 0;
  virtual void compute_bsphere(optix::float3& center, float& radius) const
  {
    optix::Aabb bbox = compute_bbox();
    center = bbox.center();
    radius = length(bbox.extent())*0.5f;
  }
  virtual optix::Aabb get_primitive_bbox(unsigned int prim_idx) const { return compute_bbox(); }
  virtual unsigned int get_no_of_primitives() const { return 1; }
};

#endif // OBJECT3D_H