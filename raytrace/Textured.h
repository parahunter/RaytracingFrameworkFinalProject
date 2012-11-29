// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef TEXTURED_H
#define TEXTURED_H

#include <map>
#include <string>
#include <optix_world.h>
#include "HitInfo.h"
#include "Texture.h"
#include "Reflectance.h"

class Texture;

class Textured : public Reflectance
{
public:
  Textured() : texs(0) { }

  void set_textures(std::map<std::string, Texture*>& textures) { texs = &textures; }

protected:
  virtual optix::float3 get_emission(const HitInfo& hit) const;
  virtual optix::float3 get_diffuse(const HitInfo& hit) const;

  std::map<std::string, Texture*>* texs;
};

#endif // TEXTURED_H
