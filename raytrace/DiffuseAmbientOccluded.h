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
	optix::float3 doRejectionSampling(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
	optix::float3 doWeightedCosineSampling(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
	optix::float3 DiffuseAmbientOccluded::doWeightedCosineSamplingWithEnvironmentSampling(const optix::Ray& r, HitInfo& hit, bool emit = true) const;
	bool sampleHemisphere(Ray& r, HitInfo& hemiHit, HitInfo& surfaceHit) const;
	bool sampleCosineWeightedHemisphere(Ray& r, HitInfo& hemiHit, HitInfo& surfaceHit) const;
};


