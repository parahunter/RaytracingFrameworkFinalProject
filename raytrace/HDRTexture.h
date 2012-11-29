// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef HDRTEXTURE_H
#define HDRTEXTURE_H

#include <optix_world.h>
#include "Texture.h"

class HDRTexture : public Texture
{
public:
  void load_hdr(const char* filename);

protected:
  optix::float4 look_up(unsigned int idx);
  float convert(unsigned char c, int e);
};

#endif // HDRTEXTURE_H
