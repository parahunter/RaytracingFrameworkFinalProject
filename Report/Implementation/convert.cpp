float HDRTexture::convert(unsigned char c, int e)
{	
	return ((float)c + .5) * pow(2.0f, e - 8);
}