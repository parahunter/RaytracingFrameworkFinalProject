bool DiffuseAmbientOccluded::sampleHemisphere(Ray& r, HitInfo& hemiHit, HitInfo& surfaceHit) const
{
	float x,y,z;
	while(true)
	{
		// get a random point in a unit cube around the point 
		x = randomizer.mt_random() * 2 - 1;
		y = randomizer.mt_random() * 2 - 1;
		z = randomizer.mt_random() * 2 - 1;
		
		// reject those outside of a unit sphere
		if(x*x + y*y + z*z > 1)
			continue;
		// reject the lower hemisphere
		if(dot(make_float3(x,y,z), surfaceHit.shading_normal) < 0)
			continue;

		break;
	}
	
	// create a ray in the sampled direction
	float3 dir = normalize(make_float3(x, y, z));

	r.direction = dir;
	r.origin = surfaceHit.position;
	r.tmin = 0.001f;
	r.tmax = 99999;

	// return false if it hit anyting
	if (tracer -> trace_to_any(r, hemiHit))
		return false;
	
  return true;
}