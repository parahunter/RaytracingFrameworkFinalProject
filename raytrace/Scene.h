// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <map>
#include <optix_world.h>
#include "ObjMaterial.h"
#include "Object3D.h"
#include "TriMesh.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Shader.h"
#include "HitInfo.h"
#include "BspTree.h"
#include "Texture.h"

class Light;
class RayTracer;

class Scene
{
public:
  Scene(Camera* c) : cam(c), shaders(10, static_cast<Shader*>(0)), redraw(true) { }
  ~Scene();

  // Accessors
  void set_shader(int model, Shader* s);
  const std::vector<Light*>& get_lights() const { return lights; }
  const std::vector<const TriMesh*>& get_meshes() const { return meshes; }
  const Shader* get_shader(const HitInfo& hit) const;
  Camera* get_camera() { return cam; }
  void get_bsphere(optix::float3& c, float& r) const;
  std::map<std::string, Texture*>& get_textures() { return textures; }

  // Loaders
  void load_mesh(const std::string& filename, const optix::Matrix4x4& transform = optix::Matrix4x4::identity());
  void load_texture(const ObjMaterial& mat, bool is_sphere = false);
  void load_textures();
  void add_plane(const optix::float3& position, const optix::float3& normal, const std::string& mtl_file, unsigned int idx = 0, float tex_scale = 1.0f);
  void add_sphere(const optix::float3& center, float radius, const std::string& mtl_file, unsigned int idx = 0);
  void add_triangle(const optix::float3& v0, const optix::float3& v1, const optix::float3& v2, const std::string& mtl_file, unsigned int idx = 0);

  // Light handling
  void add_light(Light* light) { if(light) lights.push_back(light); }
  unsigned int extract_area_lights(RayTracer* tracer, unsigned int samples_per_light = 1);
  void toggle_shadows();

  // Draw
  void draw();
  void redo_display_list() { redraw = true; }
  bool is_redoing_display_list() { return redraw; }

  // Ray intersection
  void init_accelerator();
  bool closest_hit(optix::Ray& r, HitInfo& hit) const { return acc.closest_hit(r, hit); }
  bool any_hit(optix::Ray& r, HitInfo& hit) const { return acc.any_hit(r, hit); }

  // Material classification
  bool is_specular(const ObjMaterial* m) const;

private:
  void draw_mesh(const TriMesh* mesh) const;
  void draw_plane(const Plane* plane) const;
  void draw_sphere(const Sphere* sphere) const;
  void draw_triangle(const Triangle* triangle) const;

  std::map<std::string, Texture*> textures;
  std::vector<Light*> lights;
  std::vector<const TriMesh*> light_meshes;
  std::vector<unsigned int> extracted_lights;
  std::vector<const TriMesh*> meshes;
  std::vector<const Plane*> planes;
  std::vector<const Sphere*> spheres;
  std::vector<const Triangle*> triangles;
  std::vector<Object3D*> objects;
  std::vector<optix::Matrix4x4> transforms;
  BspTree acc;
  optix::Aabb bbox;
  Camera* cam;
  std::vector<Shader*> shaders;
  bool redraw;
};

#endif // SCENE_H
