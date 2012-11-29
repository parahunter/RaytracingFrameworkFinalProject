// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef FRESNEL_H
#define FRESNEL_H

inline double fresnel_r_s(double cos_theta1, double cos_theta2, double ior1, double ior2)
{
  // Compute the perpendicularly polarized component of the Fresnel reflectance
	
  return (ior1*cos_theta1 - ior2*cos_theta2) / (ior1*cos_theta1 + ior2*cos_theta2);
}

inline double fresnel_r_p(double cos_theta1, double cos_theta2, double ior1, double ior2)
{
  // Compute the parallelly polarized component of the Fresnel reflectance
  return (ior2*cos_theta1 - ior1*cos_theta2) / (ior2*cos_theta1 + ior1*cos_theta2);
}

inline double fresnel_R(double cos_theta1, double cos_theta2, double ior1, double ior2)
{
  // Compute the Fresnel reflectance using fresnel_r_s(...) and fresnel_r_p(...)

  return 0.5 * ( pow( fresnel_r_s(cos_theta1, cos_theta2, ior1,ior2), 2) + pow( fresnel_r_p(cos_theta1, cos_theta2, ior1,ior2), 2) );
}

#endif // FRESNEL_H
