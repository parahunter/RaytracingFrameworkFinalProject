optix::float3 DiffuseAmbientOccluded::doRejectionSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_d = get_diffuse(hit);

	float3 avgUnoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	// define number of rays that are sampled
	const int numOfRays = 20;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		// for each ray perform rejection sampling
		if(sampleHemisphere(hemiRay, hemiHit, hit))
		{
			// add direction of each ray that survived rejection sampling
			// and count them to compute the average direction
			avgUnoccluded += hemiRay.direction;
			numOfUnoccluded++;
		}
	}
	//compute the average direction of unoccluded rays and accesability value at given point
	float3 averageNormal = normalize(avgUnoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;
	
	// return diffuse 
	return M_1_PIf * accessability * rho_d; // 1/PI = BRDF value
}