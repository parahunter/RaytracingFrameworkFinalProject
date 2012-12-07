#include "DiffuseAmbientOccluded.h"
#include "mt_random.h"
#include "HitInfo.h"
#include "RayTracer.h"

optix::float3 DiffuseAmbientOccluded::shade(const optix::Ray& r, HitInfo& hit, bool emit) const
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
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	int numOfRays = 1;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		if(sampleHemisphere(hemiRay, hit, hit.shading_normal))
		{
			avgUoccluded += hemiRay.direction;
			++numOfUnoccluded;
		}
	}

	float3 averageNormal = avgUoccluded / numOfUnoccluded;
	float accessability = (float)numOfUnoccluded / numOfRays;

	//return make_float3(dot(hit.shading_normal, normalize(averageNormal))) * rho_d + Emission::shade(r, hit, emit);

	return optix::make_float3(accessability);
}

bool DiffuseAmbientOccluded::sampleHemisphere(Ray& r, HitInfo& hit, float3& normal) const
{
  // Sample ray direction and create ray
	float x,y,z;
	do
	{
		x = randomizer.mt_random() * 2 - 1;
		y = randomizer.mt_random() * 2 - 1;
		z = randomizer.mt_random() * 2 - 1;
	}
	while(x*x + y*y + z*z > 1 && dot(make_float3(x,y,z), normal) < 0);

	float3 dir = normalize(make_float3(x, y, z));
	
  // Trace ray
	r.direction = hit.shading_normal;// dir;
	r.origin = hit.position;
	r.tmin = 0.00001f;
	r.tmax = 100;

	if (tracer -> trace_to_closest(r, hit))
		return false;

  return true;
}

DiffuseAmbientOccluded::~DiffuseAmbientOccluded(void)
{

}
