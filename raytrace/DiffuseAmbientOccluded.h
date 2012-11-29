#pragma once

#include "Lambertian.h"
#include <optix_world.h>
#include "RayTracer.h"

using namespace optix;

class DiffuseAmbientOccluded : public Lambertian
{
public:

	DiffuseAmbientOccluded(RayTracer* const tracerToUse, const std::vector<Light*>& light_vector) : Lambertian(light_vector), tracer(tracerToUse) {}
	
	virtual optix::float3 shade(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
	
	~DiffuseAmbientOccluded(void);

private:
	RayTracer* tracer;
	bool DiffuseAmbientOccluded::sampleHemisphere(Ray& r, HitInfo& hit, float3& normal) const;
};


