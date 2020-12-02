/*
 * Copyright (c) 2020 FUJITSU LIMITED
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
