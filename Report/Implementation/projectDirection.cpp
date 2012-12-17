void SphereTexture::project_direction(const float3& d, float& u, float& v) const
{
	// handling denominator = 0
	float dxdy = sqrt(d.x * d.x + d.y * d.y);
	if (dxdy < .001f)
	{
		u = v = .5;
		return;
	}
	
	float r = .5 * M_1_PIf * acos(-d.z)/dxdy; 
	
	u = .5 + r * d.x;
	v = .5 + r * d.y;
}