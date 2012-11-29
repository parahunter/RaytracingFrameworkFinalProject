// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef INT_POW_H
#define INT_POW_H

/// Integer power function with O(log(n)) complexity
template<class T>
inline T int_pow(T a, unsigned int n)
{
  T result = static_cast<T>(1);
  for(; n > 0; n >>= 1)
  {
    if(n & 1) result = result*a;
    a *= a;
  }
  return result;
}

#endif // INT_POW_H