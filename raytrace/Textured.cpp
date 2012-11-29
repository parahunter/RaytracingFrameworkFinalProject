// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "Texture.h"
#include "Textured.h"

using namespace optix;

float3 Textured::get_emission(const HitInfo& hit) const
{
  if(!texs)
    return Emission::get_emission(hit);

  const ObjMaterial* m = hit.material;
  if(m)
  {
    float3 emission = make_float3(m->ambient[0], m->ambient[1], m->ambient[2]);
    const Texture* tex = m->has_texture ? (*texs)[m->tex_name] : 0;
    if(tex && tex->has_texture())
    {
      float3 reduced_emission;
      reduced_emission.x = m->diffuse[0] > 0.0f ? emission.x/m->diffuse[0] : 0.0f;
      reduced_emission.y = m->diffuse[1] > 0.0f ? emission.y/m->diffuse[1] : 0.0f;
      reduced_emission.z = m->diffuse[2] > 0.0f ? emission.z/m->diffuse[2] : 0.0f;
      return reduced_emission*make_float3(tex->sample_linear(hit.texcoord));
    }
    return emission;
  }
  return make_float3(0.2f);
}

float3 Textured::get_diffuse(const HitInfo& hit) const
{
  if(!texs)
    return Reflectance::get_diffuse(hit);

  const ObjMaterial* m = hit.material;
  if(m)
  {
    const Texture* tex = m->has_texture ? (*texs)[m->tex_name] : 0;
    if(tex && tex->has_texture())
      return make_float3(tex->sample_linear(hit.texcoord));      
    return make_float3(m->diffuse[0], m->diffuse[1], m->diffuse[2]);
  }
  return make_float3(0.8f);
}
