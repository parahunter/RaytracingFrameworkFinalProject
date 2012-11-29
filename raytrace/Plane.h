// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef PLANE_H
#define PLANE_H

#include <optix_world.h>
#include "ObjMaterial.h"
#include "HitInfo.h"
#include "Object3D.h"

class Plane : public Object3D
{
public:
  Plane(const optix::float3& origin, 
        const optix::float3& normal, 
        const ObjMaterial& obj_material, 
        float texcoord_scale = 1.0f)
    : position(origin), onb(normalize(normal)),
      material(obj_material), tex_scale(texcoord_scale)
  { 
    d = -dot(origin, onb.m_normal);
  }

  virtual bool intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
  virtual void transform(const optix::Matrix4x4& m);
  virtual optix::Aabb compute_bbox() const;

  const optix::float3& get_origin() const { return position; }
  const optix::float3& get_normal() const { return onb.m_normal; }
  const optix::float3& get_tangent() const { return onb.m_tangent; }
  const optix::float3& get_binormal() const { return onb.m_binormal; }
  const ObjMaterial& get_material() const { return material; }

private:
  optix::float3 position;
  optix::Onb onb;
  float d;
  ObjMaterial material;
  float tex_scale;
};

#endif // PLANE_H