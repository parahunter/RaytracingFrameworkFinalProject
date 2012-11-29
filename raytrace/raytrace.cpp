// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include "RenderEngine.h"

int main(int argc, char** argv)
{
  render_engine.init_GLUT(argc, argv);
  render_engine.load_files(argc, argv);

  render_engine.init_GL();
  render_engine.init_texture();
  render_engine.init_tracer();

  glutMainLoop();
  return 0;
}