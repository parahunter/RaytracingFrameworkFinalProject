optix::float3 DiffuseAmbientOccluded::doWeightedCosineSamplingWithEnvironmentSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_a = get_emission(hit);
	float3 rho_d = get_diffuse(hit);

	float3 avgUnoccluded = make_float3(0,0,0);
	int numOfUnoccluded = 0;

	const int numOfRays = 200;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		if(sampleCosineWeightedHemisphere(hemiRay, hemiHit, hit))
		{
			avgUnoccluded += hemiRay.direction;
			numOfUnoccluded++;
		}
	}

	float3 averageNormal = normalize(avgUnoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;

	// sample the environment in the average unoccluded direction
	float3 hdrValue = tracer -> get_background(averageNormal);

	return M_1_PIf * accessability * rho_d * hdrValue;
}