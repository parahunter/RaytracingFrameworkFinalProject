optix::float3 DiffuseAmbientOccluded::doRejectionSampling(const optix::Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_d = get_diffuse(hit);

	float3 avgUoccluded = make_float3(0,0,0);
	float numOfUnoccluded = 0;
	
	// define number of rays that are sampled
	int numOfRays = 300;
	for(int i = 0 ; i < numOfRays ; i++)
	{
		Ray hemiRay = r;
		HitInfo hemiHit;
		if(sampleHemisphere(hemiRay, hemiHit, hit))
		{
			avgUoccluded += hemiRay.direction;
			numOfUnoccluded += dot(hemiRay.direction, hit.shading_normal);
		}
	}
	//compute the average direction of unoccluded rays and accesability value at given point
	float3 averageNormal = normalize(avgUoccluded / numOfUnoccluded);
	float accessability = ((float)numOfUnoccluded) / numOfRays;
	
	return 2 * accessability * rho_d;
}