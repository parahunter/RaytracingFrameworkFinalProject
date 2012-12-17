#include "DiffuseAmbientOccluded.h"
#include "mt_random.h"
#include "HitInfo.h"
#include "RayTracer.h"
#include "RenderEngine.h"



optix::float3 DiffuseAmbientOccluded::shade(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	// return DiffuseAmbientOccluded::doWeightedCosineSampling(r, hit, emit);
	return DiffuseAmbientOccluded::doRejectionSampling(r, hit, emit);
	// return doWeightedCosineSamplingWithEnvironmentSampling(r, hit, emit);
}

optix::float3 DiffuseAmbientOccluded::doRejectionSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int numOfRays = 200;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		if(sampleHemisphere(hemiRay, hemiHit, hit))
		{
			avgUoccluded += hemiRay.direction;
			numOfUnoccluded++;
		}
	}

	float3 averageNormal = normalize(avgUoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;

	//return make_float3(dot(hit.shading_normal, averageNormal));
	
	return M_1_PIf * accessability * rho_d;// + Emission::shade(r, hit, emit);// + Lambertian::shade(r, hit, emit);// 

}

optix::float3 DiffuseAmbientOccluded::doWeightedCosineSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int numOfRays = 50;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		if(sampleCosineWeightedHemisphere(hemiRay, hemiHit, hit))
		{
			avgUoccluded += hemiRay.direction;
			numOfUnoccluded++;
		}
	}

	float3 averageNormal = normalize(avgUoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;

	//return make_float3(dot(hit.shading_normal, averageNormal));
	
	return M_1_PIf * accessability * rho_d;// * render_engine.get_background_color();// + Lambertian::shade(r, hit, emit);// + Emission::shade(r, hit, emit);
}

optix::float3 DiffuseAmbientOccluded::doWeightedCosineSamplingWithEnvironmentSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int numOfRays = 200;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		if(sampleCosineWeightedHemisphere(hemiRay, hemiHit, hit))
		{
			avgUoccluded += hemiRay.direction;
			numOfUnoccluded++;
		}
	}

	float3 averageNormal = normalize(avgUoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;

	//return make_float3(dot(hit.shading_normal, averageNormal));
	// accessability * rho_d *
	// std::cout <<  accessability * rho_d * tracer -> get_background(averageNormal) << std::endl;

	float3 hdrValue = tracer -> get_background(averageNormal);
	float3 retVal = accessability * rho_d * hdrValue;

	return retVal;// + Lambertian::shade(r, hit, emit);// + Emission::shade(r, hit, emit);
}

float3 project(float3 v, float3 w)
{
	return (dot(v, w) / dot(w, w)) * w;
}

bool DiffuseAmbientOccluded::sampleCosineWeightedHemisphere(Ray& ray, HitInfo& hemiHit, HitInfo& surfaceHit) const
{
	optix::Onb onb = optix::Onb(surfaceHit.shading_normal);

	float phi, r;
	phi = randomizer.mt_random() * M_PIf * 2;
	r = randomizer.mt_random();

	float x = r*cos(phi);
	float y = r*sin(phi);
	float z = sqrt(1-r*r);

	float3 dir = x*onb.m_tangent + y*onb.m_binormal + z*onb.m_normal;

	dir = normalize(dir);
	
	ray.direction = dir;
	ray.origin = surfaceHit.position;
	ray.tmin = 0.001f;
	ray.tmax = 99999;

	if (tracer -> trace_to_any(ray, hemiHit))
		return false;
	
  return true;
}

bool DiffuseAmbientOccluded::sampleHemisphere(Ray& r, HitInfo& hemiHit, HitInfo& surfaceHit) const
{
  // Sample ray direction and create ray
	float x,y,z;
	while(true)
	{
		x = randomizer.mt_random() * 2 - 1;
		y = randomizer.mt_random() * 2 - 1;
		z = randomizer.mt_random() * 2 - 1;

		if(x*x + y*y + z*z > 1)
			continue;

		if(dot(make_float3(x,y,z), surfaceHit.shading_normal) < 0)
			continue;

		break;
	}

	float3 dir = normalize(make_float3(x, y, z));

  // Trace ray
	r.direction = dir;
	r.origin = surfaceHit.position;
	r.tmin = 0.001f;
	r.tmax = 99999;

	if (tracer -> trace_to_any(r, hemiHit))
		return false;
	
  return true;
}

DiffuseAmbientOccluded::~DiffuseAmbientOccluded(void)
{

}
