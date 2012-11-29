// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef PARTICLE_TRACER
#define PARTICLE_TRACER

#include <optix_world.h>
#include "HitInfo.h"
#include "PhotonMap.h"
#include "Light.h"
#include "RayTracer.h"

class ParticleTracer : public RayTracer
{
public:
  ParticleTracer(unsigned int w, 
                 unsigned int h, 
                 Scene* s, 
                 unsigned int max_no_of_particles,
                 unsigned int pixel_subdivs = 1)
    : RayTracer(w, h, s, pixel_subdivs), caustics(max_no_of_particles)
  { }

  void build_maps(int no_of_caustic_particles, unsigned int max_no_of_shots = 500000);
  void draw_caustics_map();

  optix::float3 caustics_irradiance(const HitInfo& hit, float max_distance, int no_of_particles);

protected:
  void trace_particle(const Light* light, const unsigned int caustics_done);
  optix::float3 get_diffuse(const HitInfo& hit) const;
  optix::float3 get_transmittance(const HitInfo& hit) const;

  PhotonMap<> caustics;
};

#endif // PARTICLE_TRACER
