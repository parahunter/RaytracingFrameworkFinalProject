bool DiffuseAmbientOccluded::sampleCosineWeightedHemisphere(Ray& ray, HitInfo& hemiHit, HitInfo& surfaceHit) const
{
	// get an orthonormal basis at the given point
	optix::Onb onb = optix::Onb(surfaceHit.shading_normal);

	// get a random point from a unit circle around the point using polar coordinates
	float phi, r;
	phi = randomizer.mt_random() * M_PIf * 2;
	r = randomizer.mt_random();

	// "raise" the point from the circle to the hemisphere obove it
	float x = r*cos(phi);
	float y = r*sin(phi);
	float z = sqrt(1-r*r);

	// transform the coordinates to the onb coordinate system 
	float3 dir = x*onb.m_tangent + y*onb.m_binormal + z*onb.m_normal;

	dir = normalize(dir);
	
	ray.direction = dir;
	ray.origin = surfaceHit.position;
	ray.tmin = 0.001f;
	ray.tmax = 99999;

	// trace the ray in the sampled direction, return false if it hit anything
	if (tracer -> trace_to_any(ray, hemiHit))
		return false;
	
  return true;
}