// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef SPHERE_H
#define SPHERE_H

#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "Object3D.h"

class Sphere : public Object3D
{
public:
  Sphere(const optix::float3& sphere_center, float sphere_radius, const ObjMaterial& obj_material)
    : center(sphere_center), radius(sphere_radius), material(obj_material)
  { }

  virtual bool intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
  virtual void transform(const optix::Matrix4x4& m);
  virtual optix::Aabb compute_bbox() const;
  virtual void compute_bsphere(optix::float3& bcenter, float& bradius) const
  {
    bcenter = center;
    bradius = radius;
  }

  const optix::float3& get_center() const { return center; }
  float get_radius() const { return radius; }
  const ObjMaterial& get_material() const { return material; }

private:
  optix::float3 center;
  float radius;
  ObjMaterial material;
};

#endif // SPHERE_H