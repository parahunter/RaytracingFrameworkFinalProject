// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <optix_world.h>
#include "../SOIL/SOIL.h"

class Texture
{
public:
  Texture() : width(0), height(0), data(0), fdata(0), clamp(false), tex_handle(0), tex_target(GL_TEXTURE_2D) { }
  ~Texture() { SOIL_free_image_data(data); delete [] fdata; }

  // Load texture from file
  void load(const char* filename);

  // Load texture from OpenGL texture
  void load(GLenum target, GLuint texture);

  // Clear texture data
  void clear() { SOIL_free_image_data(data); data = 0; delete [] fdata; fdata = 0; }

  // Was a texture loaded yet
  bool has_texture() const { return fdata != 0; }

  // Look up the texel using texture space coordinates
  virtual optix::float4 sample_nearest(const optix::float3& texcoord) const;
  virtual optix::float4 sample_linear(const optix::float3& texcoord) const;

  // Clamp the texture
  void clamp_to_edge() { clamp = true; }

  // Repeat the texture
  void repeat() { clamp = false; }

  // OpenGL
  void bind() const { glBindTexture(tex_target, tex_handle); }
  void enable() const { glEnable(tex_target); }
  void disable() const { glDisable(tex_target); }

protected:
  optix::float4 look_up(unsigned int idx) const;
  float convert(unsigned char c) const;

  // Texture dimensions
  int width;
  int height;

  // Pointers to image data
  unsigned char* data;
  optix::float4* fdata;

  // OpenGL texture info
  GLuint tex_handle;
  GLenum tex_target;

  // If clamp is false the texture is repeated
  bool clamp;

  // Bytes per pixel
  int channels;
};

#endif // TEXTURE_H
