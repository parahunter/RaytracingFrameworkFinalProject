// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <GL/glut.h>
#include <optix_world.h>
#include "../SOIL/stb_image_write.h"
#include "string_utils.h"
#include "Timer.h"
#include "mt_random.h"
#include "Directional.h"
#include "PointLight.h"
#include "RenderEngine.h"

#ifdef _OPENMP
  #include <omp.h>
#endif

using namespace std;
using namespace optix;

RenderEngine render_engine;

namespace
{
  // String utilities
	void lower_case(char& x) { x = tolower(x); }

	inline void lower_case_string(std::string& s)
	{
    for_each(s.begin(), s.end(), lower_case);
	}
}

//////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////

RenderEngine::RenderEngine() 
  : win(optix::make_uint2(512, 512)),                        // Default window size
    res(optix::make_uint2(512, 512)),                        // Default render resolution
    image(res.x*res.y),
    image_tex(0),
    scene(&cam),
    filename("out.ppm"),                                     // Default output file name
    tracer(res.x, res.y, &scene, 100000),                    // Maximum number of photons in map
    max_to_trace(500000),                                    // Maximum number of photons to trace
    caustics_particles(0),                               // Desired number of caustics photons
    done(false), 
    light_pow(optix::make_float3(M_PIf)),                    // Power of the default light
    light_dir(optix::make_float3(-1.0f)),                    // Direction of the default light
    default_light(&tracer, light_pow, light_dir),            // Construct default light
    use_default_light(true),                                 // Choose whether to use the default light or not
    shadows_on(true),
    background(optix::make_float3(0.1f, 0.3f, 0.6f)),        // Background color
    bgtex_filename("../models/grace_probe.hdr"),
	// bgtex_filename(""), // Background texture file name
    current_shader(0),
    lambertian(scene.get_lights()),
    photon_caustics(&tracer, scene.get_lights(), 1.0f, 50),  // Max distance and number of photons to search for
    glossy(&tracer, scene.get_lights()),
    mirror(&tracer),
    transparent(&tracer),
    volume(&tracer),
    glossy_volume(&tracer, scene.get_lights()),
    tone_map(2.5),                                            // Gamma for gamma correction
	diffAmbientOccluded(&tracer, scene.get_lights())
{ 
  shaders.push_back(&reflectance);                           // number key 0 (reflectance only)
  shaders.push_back(&lambertian);                            // number key 1 (direct lighting)
  shaders.push_back(&photon_caustics);                       // number key 2 (photon map caustics)
  shaders.push_back(&diffAmbientOccluded);					 // number key 3 (ambient occlusion)
}

void RenderEngine::load_files(int argc, char** argv)
{
  if(argc > 1)
  {
    for(int i = 1; i < argc; ++i)
    {
      // Retrieve filename without path
      list<string> path_split;
      split(argv[i], path_split, "\\");
      filename = path_split.back();
      if(filename.find("/") != filename.npos)
      {
        path_split.clear();
        split(filename, path_split, "/");
        filename = path_split.back();
      }
      lower_case_string(filename);
      Matrix4x4 transform = Matrix4x4::identity();scene.add_plane(make_float3(0.0f, 0.0f, 0.0f), make_float3(0.0f, 1.0f, 0.0f), "../models/default_scene.mtl", 1, 0.02f);
    
      // Special rules for some meshes
      if(char_traits<char>::compare(filename.c_str(), "cornell", 7) == 0)
        transform = Matrix4x4::scale(make_float3(0.025f))*Matrix4x4::rotate(M_PIf, make_float3(0.0f, 1.0f, 0.0f));
      else if(char_traits<char>::compare(filename.c_str(), "bunny", 5) == 0)
        transform = Matrix4x4::translate(make_float3(-3.0f, -0.85f, -8.0f))*Matrix4x4::scale(make_float3(25.0f));
      else if(char_traits<char>::compare(filename.c_str(), "justelephant", 12) == 0)
        transform = Matrix4x4::translate(make_float3(-10.0f, 3.0f, -2.0f))*Matrix4x4::rotate(0.5f, make_float3(0.0f, 1.0f, 0.0f));
     
      // Load the file into the scene
      scene.load_mesh(argv[i], transform);
    }
    init_view();
  }
  else
  {
    // Insert default scene

	//scene.add_plane(make_float3(0.0f, -10.0f, 0.0f), make_float3(0.0f, 1.0f, 0.0f), "../models/default_scene.mtl", 1, 0.02f);
    
	//scene.add_sphere(make_float3(-4.0f, 0.5f, -4.0f), 0.3f, "../models/default_scene.mtl", 3);
	
	/*
	float dist = 2.0f;
	float offset = - dist*2;
	for(int z = 0 ; z < 4 ; z++)
	{
		for(int x = 0 ; x < 4 - z ;x++)
		{
			for(int y = 0 ; y < 4 - z ;y++)
			{
					scene.add_sphere(make_float3(offset + dist * x, dist*4 - dist*z,offset + dist * y), 0.6f, "../models/default_scene.mtl", 3);
			}
		}

		offset += dist/2;
	}
	*/
	
	//our scene
	scene.add_plane(make_float3(0.0f, 0.0f, 0.0f), make_float3(0.0f, 1.0f, 0.0f), "../models/default_scene.mtl", 1, 0.02f);
    scene.add_light(new PointLight(&tracer, make_float3(100 * M_PIf), make_float3(0.0f, 10.0f, 0.0f)));
    cam.set(make_float3(7.5f, 4.5f, 7.0f), make_float3(0.0f, 3, 0.0f), make_float3(0.0f, 1.0f, 0.0f), 1.0f);

	for(int x = -2 ; x < 2 ;x++)
	{
		for(int y = -2 ; y < 2 ;y++)
		{
			for(int z = 1 ; z < 4 ; z++)
				 scene.add_sphere(make_float3(2.0f * x, 1.5f*z, 2.0f * y), 0.6f, "../models/default_scene.mtl", 3);
		}
	}
	/*
	for(int x = 0 ; x < 3 ;x++)
	{
		for(int y = 0 ; y < 3 ;y++)
		{
			float offset = -3;
			 scene.add_sphere(make_float3(offset + 2.0f * x, 1.8f, offset + 2.0f * y), 0.6f, "../models/default_scene.mtl", 3);
		}
	}*/	
	/*
	//scene.add_triangle(make_float3(-0.2f, 0.1f, 0.9f), make_float3(0.2f, 0.1f, 0.9f), make_float3(-0.2f, 0.1f, -0.1f), "../models/default_scene.mtl", 3);
	
    scene.add_plane(make_float3(0.0f, 0.0f, 0.0f), make_float3(0.0f, 1.0f, 0.0f), "../models/default_scene.mtl", 1, 0.02f);
    scene.add_sphere(make_float3(0.0f, 0.5f, 0.0f), 0.3f, "../models/default_scene.mtl", 2);
    scene.add_triangle(make_float3(-0.2f, 0.1f, 0.9f), make_float3(0.2f, 0.1f, 0.9f), make_float3(-0.2f, 0.1f, -0.1f), "../models/default_scene.mtl", 3);
    scene.add_light(new PointLight(&tracer, make_float3(M_PIf), make_float3(0.0f, 1.0f, 0.0f)));
    
	cam.set(make_float3(2.0f, 1.5f, 2.0f), make_float3(0.0f, 0.5, 0.0f), make_float3(0.0f, 1.0f, 0.0f), 1.0f);
	*/
  }
}

void RenderEngine::init_GLUT(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(win.x, win.y);
  glutCreateWindow("02562 Render Framework");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
}

void RenderEngine::init_GL()
{
  glEnable(GL_CULL_FACE);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void RenderEngine::init_view()
{
  float3 c;
  float r;
  scene.get_bsphere(c, r);
  r *= 1.75f;

  // Initialize corresponding camera for tracer
  cam.set(c + make_float3(0.0f, 0.0f, r), c, make_float3(0.0f, 1.0f, 0.0f), 1.0f);
}

void RenderEngine::init_tracer()
{
  clear_image();

  // Insert background texture/color
  tracer.set_background(background);
  if(!bgtex_filename.empty())
  {
    list<string> dot_split;
    split(bgtex_filename, dot_split, ".");
    if(dot_split.back() == "hdr")
      bgtex.load_hdr(bgtex_filename.c_str());
    else
      bgtex.load(bgtex_filename.c_str());
    tracer.set_background(&bgtex);
  }

  // Set shaders
  scene.set_shader(0, shaders[current_shader]); // shader for illum 0 (chosen by number key)
  scene.set_shader(1, shaders[current_shader]); // shader for illum 1 (chosen by number key)
  scene.set_shader(2, &glossy);                 // shader for illum 2 (calls lambertian until implemented)
  scene.set_shader(3, &mirror);                 // shader for illum 3
  scene.set_shader(4, &transparent);            // shader for illum 4
  scene.set_shader(11, &volume);                // shader for illum 11
  scene.set_shader(12, &glossy_volume);         // shader for illum 12
  
  // Load material textures
  scene.load_textures();

  // Add polygons with an ambient material as area light sources
  unsigned int lights_in_scene = scene.extract_area_lights(&tracer, 8);  // Set number of samples per light source here

  // If no light in scene, add default light source (shadow off)
  if(lights_in_scene == 0 && use_default_light)
  {
    cout << "Adding default light: " << default_light.describe() << endl;
    scene.add_light(&default_light);
  }

  // Build acceleration data structure
  Timer timer;
  cout << "Building acceleration structure...";
  timer.start();
  scene.init_accelerator();
  timer.stop();
  cout << "(time: " << timer.get_time() << ")" << endl; 

  // Build photon maps
  cout << "Building photon maps... " << endl;
  timer.start();
  tracer.build_maps(caustics_particles, max_to_trace);
  timer.stop();
  cout << "Building time: " << timer.get_time() << endl;
}

void RenderEngine::init_texture()
{
  if(!glIsTexture(image_tex))
    glGenTextures(1, &image_tex);
  glBindTexture(GL_TEXTURE_2D, image_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // load the texture image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res.x, res.y, 0, GL_RGB, GL_FLOAT, &image[0].x);
}


//////////////////////////////////////////////////////////////////////
// Rendering
//////////////////////////////////////////////////////////////////////

void RenderEngine::apply_tone_map()
{
  if(done)
  {
    tone_map.apply(&image[0].x, res.x, res.y, 3);
    init_texture();
    glutPostRedisplay();
  }
}

void RenderEngine::unapply_tone_map()
{
  if(done)
  {
    tone_map.unapply(&image[0].x, res.x, res.y, 3);
    init_texture();
    glutPostRedisplay();
  }
}

void RenderEngine::add_textures()
{
  reflectance.set_textures(scene.get_textures());
  lambertian.set_textures(scene.get_textures());
  glossy.set_textures(scene.get_textures());
  glossy_volume.set_textures(scene.get_textures());
  photon_caustics.set_textures(scene.get_textures());
}

void RenderEngine::render()
{
  cout << "Raytracing";
  Timer timer;
  timer.start();	

  #pragma omp parallel for private(randomizer)

  for(int j = 0; j < static_cast<int>(res.y); ++j)
  {
		for(int k = 0 ; k <static_cast<int>(res.x) ; k++)
		{
			image[j*res.x + k] = tracer.compute_pixel(k,j);
		}
    if(((j + 1) % 50) == 0) 
      cerr << ".";
  }
  timer.stop();
  cout << " - " << timer.get_time() << " secs " << endl;

  init_texture();
  done = true;
}

//////////////////////////////////////////////////////////////////////
// Export/import
//////////////////////////////////////////////////////////////////////

void RenderEngine::save_as_bitmap()
{
  string png_name = "out.png";
  if(!filename.empty())
  {
    list<string> dot_split;
    split(filename, dot_split, ".");
    png_name = dot_split.front() + ".png";
  }
  unsigned char* data = new unsigned char[res.x*res.y*3];
  for(unsigned int j = 0; j < res.y; ++j)
    for(unsigned int i = 0; i < res.x; ++i)
    {
      unsigned int d_idx = (i + res.x*j)*3;
      unsigned int i_idx = i + res.x*(res.y - j - 1);
      data[d_idx + 0] = static_cast<unsigned int>(std::min(image[i_idx].x, 1.0f)*255.0f + 0.5f);
      data[d_idx + 1] = static_cast<unsigned int>(std::min(image[i_idx].y, 1.0f)*255.0f + 0.5f);
      data[d_idx + 2] = static_cast<unsigned int>(std::min(image[i_idx].z, 1.0f)*255.0f + 0.5f);
    }
  stbi_write_png(png_name.c_str(), res.x, res.y, 3, data, res.x*3);
  delete [] data;
  cout << "Rendered image stored in " << png_name << "." << endl;
}


//////////////////////////////////////////////////////////////////////
// Draw functions
//////////////////////////////////////////////////////////////////////

void RenderEngine::set_gl_ortho_proj()
{
  glMatrixMode(GL_PROJECTION);	 
  glLoadIdentity();             
    
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);  
}

void RenderEngine::draw_texture()
{
  static GLfloat verts[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, image_tex);
  glEnable(GL_TEXTURE_2D);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(2, GL_FLOAT, 0, verts);
  glTexCoordPointer(2, GL_FLOAT, 0, verts);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glDisable(GL_TEXTURE_2D);
}

void RenderEngine::draw()
{ 
  if(shaders[current_shader] == &photon_caustics)
    tracer.draw_caustics_map();
  else
    scene.draw(); 
}


//////////////////////////////////////////////////////////////////////
// GLUT callback functions
//////////////////////////////////////////////////////////////////////

void RenderEngine::display()
{
  if(render_engine.is_done())
  {
    render_engine.set_gl_ortho_proj();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render_engine.draw_texture();
  }
  else
  {
    glEnable(GL_DEPTH_TEST);
    render_engine.set_gl_perspective();
    render_engine.set_gl_clearcolor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	  render_engine.set_gl_camera();
    render_engine.draw();
    glDisable(GL_DEPTH_TEST);
  }
  glutSwapBuffers();  
}

void RenderEngine::reshape(int width, int height)
{
  render_engine.set_window_size(width, height);
  glViewport(0, 0, width, height);
}

void RenderEngine::keyboard(unsigned char key, int x, int y)
{
  // The shader to be used when rendering a material is chosen 
  // by setting the "illum" property of the material. This
  // property is part of the MTL file format ("illum" is short
  // for "illumination model"). The shader to be used with
  // each illumination model is specified in the init_tracer 
  // function above.
  //
  // Number keys switch the shader used for illumination
  // models 0 and 1 to the shader at the corresponding index
  // in the array "shaders" at the top of this file.
  //
  // When you switch shaders all previous rendering results
  // will be erased!
  if(key >= 48 && key < 48 + static_cast<unsigned char>(render_engine.no_of_shaders()))
  {
    unsigned int shader_no = key - 48;
    if(shader_no != render_engine.get_current_shader())
    {
      render_engine.set_current_shader(shader_no);
      render_engine.clear_image();
      render_engine.redo_display_list();
      cout << "Switched to shader number " << shader_no << endl;
      glutPostRedisplay();
    }
  }
  switch(key)
  { 
  // Use '+' and '-' to increase or decrease the number of
  // jitter samples per pixel in a simple ray tracing
  case '+':
    render_engine.increment_pixel_subdivs();
    break;
  case '-':
    render_engine.decrement_pixel_subdivs();
    break;
  // Press '*' to apply tone mapping
  case '*':
    render_engine.apply_tone_map();
    break;
  // Press '/' to unapply tone mapping
  case '/':
    render_engine.unapply_tone_map();
    break;
  // Press 'b' to save the render result as a bitmap called out.png.
  // If obj files are loaded, the png will be named after the obj file loaded last.
  case 'b':
    render_engine.save_as_bitmap();
    break;
  // Press 'r' to start a simple ray tracing (one pass -> done).
  // To switch back to preview mode after the ray tracing is done
  // press 'r' again.
  case 'r':
    if(render_engine.is_done())
      render_engine.undo();
    else 
      render_engine.render();
    glutPostRedisplay();
    break;
  // Press 's' to toggle shadows on/off
  case 's':
    {
      bool shadows_on = render_engine.toggle_shadows();
      render_engine.clear_image();
      render_engine.redo_display_list();
      cout << "Toggled shadows " << (shadows_on ? "on" : "off") << endl;
      glutPostRedisplay();
    }
    break;
  // Press 'x' to switch on material textures.
  case 'x':
    render_engine.add_textures();
    cout << "Toggled textures on." << endl;
    break;
  // Press 'z' to zoom in.
  case 'z':
    {
      render_engine.set_focal_dist(render_engine.get_focal_dist()*1.05f);
      glutPostRedisplay();
    }
    break;
  // Press 'Z' to zoom out.
  case 'Z':
    {
      render_engine.set_focal_dist(render_engine.get_focal_dist()/1.05f);
      glutPostRedisplay();
    }
    break;

  // Press 'space' to switch between pre-view and your last tracing result.
  case 32:
    render_engine.undo();
    glutPostRedisplay();
    break;
  // Press 'esc' to exit the program.
  case 27:
    exit(0);
  }
}

void RenderEngine::set_current_shader(unsigned int shader) 
{ 
  current_shader = shader;
  for(int i = 0; i < 2; ++i)
    scene.set_shader(i, shaders[current_shader]);
}
