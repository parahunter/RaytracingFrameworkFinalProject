#include "DiffuseAmbientOccluded.h"
#include "mt_random.h"
#include "HitInfo.h"
#include "RayTracer.h"



optix::float3 DiffuseAmbientOccluded::shade(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	return doRejectionSampling(r, hit, emit);
}

optix::float3 DiffuseAmbientOccluded::doRejectionSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	/*
	For each triangle {
  Compute center of triangle
  Generate set of rays over the hemisphere there
  Vector avgUnoccluded = Vector(0, 0, 0);
  int numUnoccluded = 0;
  For each ray {
    If (ray doesn't intersect anything) {
      avgUnoccluded += ray.direction;
      ++numUnoccluded;
    }
  }
  avgUnoccluded = normalize(avgUnoccluded);
  accessibility = numUnoccluded / numRays;

*/
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int numOfRays = 20;
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
	
	return M_1_PIf * accessability * rho_a;// + Lambertian::shade(r, hit, emit);// + Emission::shade(r, hit, emit);

}

optix::float3 DiffuseAmbientOccluded::doMonteCarloSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int hemicubeSize = 2;

	float3 hemicubeZ = hit.shading_normal;
	float3 hemicubeY = hit.shading_normal - project(hit.shading_normal, r.direction);
	float3 hemicubeX = cross(hemicubeY, hemicubeZ);



	float3 averageNormal = normalize(avgUoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;

	//return make_float3(dot(hit.shading_normal, averageNormal));
	
	return M_1_PIf * accessability * rho_a;// + Lambertian::shade(r, hit, emit);// + Emission::shade(r, hit, emit);
}

float3 project(float3 v, float3 w)
{
	return (dot(v, w) / dot(w, w)) * w;
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
