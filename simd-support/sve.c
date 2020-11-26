/*
 * sve.c
 * for Fujitsu A64FX SIMD instruction support
 * this code is subject to FFTW's license
 * implemented by Tetsuzo Usui 2018.8
*/

#include "kernel/ifftw.h"

#if HAVE_SVE

/* assume it is present */
  int X(have_simd_sve)(void)
  {
       return 1;
  }

#endif
