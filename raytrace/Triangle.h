// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "Object3D.h"

bool intersect_triangle(const optix::Ray& r, 
                        const optix::float3& v0, 
                        const optix::float3& v1, 
                        const optix::float3& v2, 
                        optix::float3& n,
                        float& t,
                        float& v,
                        float& w);

class Triangle : public Object3D
{
public:
  Triangle(optix::float3 vert0, 
           optix::float3 vert1, 
           optix::float3 vert2,
           const ObjMaterial& obj_material) 
    : v0(vert0), v1(vert1), v2(vert2), 
      t0(optix::make_float3(0.0f)), t1(optix::make_float3(0.0f)), t2(optix::make_float3(0.0f)), 
      material(obj_material)
  { }

  virtual bool intersect(const optix::Ray& ray, HitInfo& hit, unsigned int prim_idx) const;
  virtual void transform(const optix::Matrix4x4& m);
  virtual optix::Aabb compute_bbox() const;

  const optix::float3* get_vertices() const { return &v0; }
  const ObjMaterial& get_material() const { return material; }
  void set_texcoords(const optix::float3 tc0, const optix::float3 tc1, const optix::float3 tc2)
  {
    t0 = tc1; t1 = tc1; t2 = tc2;  
  }
  optix::float3 compute_normal() const { return optix::cross(v1 - v0, v2 - v0); }

private:
  optix::float3 v0, v1, v2;
  optix::float3 t0, t1, t2;
  ObjMaterial material;
};

#endif // TRIANGLE_H