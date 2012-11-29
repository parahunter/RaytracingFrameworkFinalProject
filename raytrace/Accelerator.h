// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <vector>
#include <optix_world.h>
#include "AccObj.h"
#include "Object3D.h"
#include "Plane.h"
#include "HitInfo.h"

class Accelerator
{
public:
  virtual ~Accelerator();
  virtual void init(const std::vector<Object3D*>& geometry, const std::vector<const Plane*>& scene_planes);
  virtual bool closest_hit(optix::Ray& r, HitInfo& hit) const;
  virtual bool any_hit(optix::Ray& r, HitInfo& hit) const;

protected:
  void closest_plane(optix::Ray& r, HitInfo& hit) const;
  bool any_plane(optix::Ray& r, HitInfo& hit) const;

  std::vector<AccObj*> primitives;
  std::vector<const Plane*> planes;
};

#endif // ACCELERATOR_H