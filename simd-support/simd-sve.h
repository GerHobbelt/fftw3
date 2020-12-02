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
 * simd-sve.h
 * for Fujitsu A64FX SIMD instruction support
 * this code is subject to FFTW's license
 * implemented by Tetsuzo Usui 2018.11
 */

#if defined(FFTW_LDOUBLE) || defined(FFTW_QUAD)
#error "ARM-SVE vector instructions only works in single or double precision"
#endif

#include <arm_sve.h>
#define SIMD_SUFFIX _sve  /* for renaming */

#ifdef FFTW_SINGLE
#  define DS(d,s) s /* single-precision option */
#  define SUFF(name) name ## _f32
#  define SUFFx(name) name ## _f32_x
#  define VZERO svdup_n_f32(0.0)
#  define PTRUE svptrue_b32
#else
#  define DS(d,s) d /* double-precision option */
#  define SUFF(name) name ## _f64
#  define SUFFx(name) name ## _f64_x
#  define VZERO svdup_n_f64(0.0)
#  define PTRUE svptrue_b64
#endif

#define VL DS(4, 8) /* SIMD complex vector length (512bit) */
#define SIMD_VSTRIDE_OKA(x) ((x) == 2)
#define SIMD_STRIDE_OKPAIR SIMD_STRIDE_OK

typedef DS(svfloat64_t, svfloat32_t) V;

#define LDK(x) x
#define DVK(var, val) V var = SUFF(svdup_n)(val)

#define FLIP_RI(x) SUFF(svzip1)(SUFF(svuzp2)(x,x),SUFF(svuzp1)(x,x))
#define VCONJ(x) DS(svneg_f64_m,svneg_f32_m)(x,pgi,x)
#define VBYI(x) SUFFx(svcadd)(pg,VZERO,x,90)

#define VADD(a,b) SUFFx(svadd)(pg, a, b)
#define VSUB(a,b) SUFFx(svsub)(pg, a, b)
#define VMUL(a,b) SUFFx(svmul)(pg, a, b)
#define VFMA(a, b, c) SUFFx(svmla)(pg, c, a, b)
#define VFMS(a, b, c) SUFFx(svnmls)(pg, c, a, b)
#define VFNMS(a, b, c) SUFFx(svmls)(pg, c, a, b)

#define VFMAI(b, c) SUFFx(svcadd)(pg, c, b, 90)
#define VFNMSI(b, c) SUFFx(svcadd)(pg, c, b, 270)
#define VFMACONJ(b,c)  VADD(VCONJ(b),c)
#define VFMSCONJ(b,c)  VSUB(VCONJ(b),c)
#define VFNMSCONJ(b,c) VSUB(c, VCONJ(b))

static inline V LDA(const R *x, INT ivs, const R *aligned_like) {
     (void)aligned_like; /* UNUSED */
     (void)ivs; /* UNUSED */
     svbool_t pg = PTRUE();
     return SUFF(svld1)(pg, x);
}
static inline void STA(R *x, V v, INT ovs, const R *aligned_like) {
     (void)aligned_like; /* UNUSED */
     (void)ovs; /* UNUSED */
     svbool_t pg = PTRUE();
     SUFF(svst1)(pg, x, v);
}

#ifdef FFTW_SINGLE
static inline V LD(const R *x, INT ivs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pgp = (ivs) ? svptrue_b32() : svptrue_pat_b32(SV_VL2);
     svint32_t vidxc = svzip1_s32(svindex_s32(0,ivs),svindex_s32(1,ivs));
     return svld1_gather_s32index_f32(pgp, x, vidxc);
}
static inline void ST(R *x, V v, INT ovs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pgp = (ovs) ? svptrue_b32() : svptrue_pat_b32(SV_VL2);
     svint32_t vidxc = svzip1_s32(svindex_s32(0,ovs),svindex_s32(1,ovs));
     svst1_scatter_s32index_f32(pgp, x, vidxc, v);
}
#else /* !FFTW_SINGLE */
static inline V LD(const R *x, INT ivs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pgp = (ivs) ? svptrue_b64() : svptrue_pat_b64(SV_VL2);
     svint64_t vidxc = svzip1_s64(svindex_s64(0,ivs),svindex_s64(1,ivs));
     return svld1_gather_s64index_f64(pgp, x, vidxc);
}
static inline void ST(R *x, V v, INT ovs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pgp = (ovs) ? svptrue_b64() : svptrue_pat_b64(SV_VL2);
     svint64_t vidxc = svzip1_s64(svindex_s64(0,ovs),svindex_s64(1,ovs));
     svst1_scatter_s64index_f64(pgp, x, vidxc, v);
}
#endif /* FFTW_SINGLE */

#define STM2(x, v, ovs, a) ST(x, v, ovs, a)
#define STN2(x, v0, v1, ovs) /* nop */

#ifdef FFTW_SINGLE
static inline void STM4(R *x, V v, INT ovs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pg = svptrue_b32();
     svint32_t vidx = svindex_s32(0,ovs);
     svst1_scatter_s32index_f32(pg, x, vidx, v);
}
#else /* !FFTW_SINGLE */
static inline void STM4(R *x, V v, INT ovs, const R *aligned_like)
{
     (void)aligned_like; /* UNUSED */
     svbool_t pg = svptrue_b64();
     svint64_t vidx = svindex_s64(0,ovs);
     svst1_scatter_s64index_f64(pg, x, vidx, v);
}
#endif /* FFTW_SINGLE */
#define STN4(x, v0, v1, v2, v3, ovs) /* no-op */

static inline V VZMUL(V tx, V sr)
{
     svbool_t pg = PTRUE();
     return SUFFx(svcmla)(pg, SUFFx(svcmla)(pg,VZERO,tx,sr,0), tx, sr, 90);
}
static inline V VZMULJ(V tx, V sr)
{
     svbool_t pg = PTRUE();
     return SUFFx(svcmla)(pg, SUFFx(svcmla)(pg,VZERO,tx,sr,0), tx, sr, 270);
}
static inline V VZMULI(V tx, V sr)
{
     svbool_t pg = PTRUE();
     V tmp = SUFFx(svcmla)(pg, SUFFx(svcmla)(pg,VZERO,tx,sr,0), tx, sr, 90);
     return VBYI(tmp);
}
static inline V VZMULIJ(V tx, V sr)
{
     svbool_t pg = PTRUE();
     V tmp = SUFFx(svcmla)(pg, SUFFx(svcmla)(pg,VZERO,tx,sr,0), tx, sr, 270);
     return VBYI(tmp);
}

/* twiddle storage #1: compact, slower */
#ifdef FFTW_SINGLE
# define VTW1(v,x) {TW_CEXP, v, x}, {TW_CEXP, v+1, x}, {TW_CEXP, v+2, x}, {TW_CEXP, v+3, x}, {TW_CEXP, v+4, x}, {TW_CEXP, v+5, x}, {TW_CEXP, v+6, x}, {TW_CEXP, v+7, x}
#else /* !FFTW_SINGLE */
# define VTW1(v,x) {TW_CEXP, v, x}, {TW_CEXP, v+1, x}, {TW_CEXP, v+2, x}, {TW_CEXP, v+3, x}
#endif /* FFTW_SINGLE */
#define TWVL1 (VL)

static inline V BYTW1(const R *t, V sr)
{
     svbool_t pg = PTRUE();
     return VZMUL(LDA(t, 2, t), sr);
}

static inline V BYTWJ1(const R *t, V sr)
{
     svbool_t pg = PTRUE();
     return VZMULJ(LDA(t, 2, t), sr);
}

/* twiddle storage #2: twice the space, faster (when in cache) */
#ifdef FFTW_SINGLE
# define VTW2(v,x)                                                              \
      {TW_COS, v  ,  x}, {TW_COS, v  , x}, {TW_COS, v+1,  x}, {TW_COS, v+1, x}, \
      {TW_COS, v+2,  x}, {TW_COS, v+2, x}, {TW_COS, v+3,  x}, {TW_COS, v+3, x}, \
      {TW_COS, v+4,  x}, {TW_COS, v+4, x}, {TW_COS, v+5,  x}, {TW_COS, v+5, x}, \
      {TW_COS, v+6,  x}, {TW_COS, v+6, x}, {TW_COS, v+7,  x}, {TW_COS, v+7, x}, \
      {TW_SIN, v  , -x}, {TW_SIN, v  , x}, {TW_SIN, v+1, -x}, {TW_SIN, v+1, x}, \
      {TW_SIN, v+2, -x}, {TW_SIN, v+2, x}, {TW_SIN, v+3, -x}, {TW_SIN, v+3, x}, \
      {TW_SIN, v+4, -x}, {TW_SIN, v+4, x}, {TW_SIN, v+5, -x}, {TW_SIN, v+5, x}, \
      {TW_SIN, v+6, -x}, {TW_SIN, v+6, x}, {TW_SIN, v+7, -x}, {TW_SIN, v+7, x}
#else /* !FFTW_SINGLE */
# define VTW2(v,x)                                                              \
      {TW_COS, v  ,  x}, {TW_COS, v  , x}, {TW_COS, v+1,  x}, {TW_COS, v+1, x}, \
      {TW_COS, v+2,  x}, {TW_COS, v+2, x}, {TW_COS, v+3,  x}, {TW_COS, v+3, x}, \
      {TW_SIN, v  , -x}, {TW_SIN, v  , x}, {TW_SIN, v+1, -x}, {TW_SIN, v+1, x}, \
      {TW_SIN, v+2, -x}, {TW_SIN, v+2, x}, {TW_SIN, v+3, -x}, {TW_SIN, v+3, x}
#endif /* FFTW_SINGLE */
#define TWVL2 (2 * VL)

static inline V BYTW2(const R *t, V sr)
{
     svbool_t pg = PTRUE();
     V si = FLIP_RI(sr);
     V tr = LDA(t, 2, 0), ti = LDA(t + 2*VL, 2, 0);
     return VFMA(tr, sr, VMUL(ti, si));
}

static inline V BYTWJ2(const R *t, V sr)
{
     svbool_t pg = PTRUE();
     V si = FLIP_RI(sr);
     V tr = LDA(t, 2, 0), ti = LDA(t + 2*VL, 2, 0);
     return VFNMS(ti, si, VMUL(tr, sr));
}

/* twiddle storage #3 */
#define VTW3(v,x) VTW1(v,x)
#define TWVL3 TWVL1

/* twiddle storage for split arrays */
#ifdef FFTW_SINGLE
# define VTWS(v,x)                                                               \
     {TW_COS, v   , x}, {TW_COS, v+1 , x}, {TW_COS, v+2 , x}, {TW_COS, v+3 , x}, \
     {TW_COS, v+4 , x}, {TW_COS, v+5 , x}, {TW_COS, v+6 , x}, {TW_COS, v+7 , x}, \
     {TW_COS, v+8 , x}, {TW_COS, v+9 , x}, {TW_COS, v+10, x}, {TW_COS, v+11, x}, \
     {TW_COS, v+12, x}, {TW_COS, v+13, x}, {TW_COS, v+14, x}, {TW_COS, v+15, x}, \
     {TW_SIN, v   , x}, {TW_SIN, v+1 , x}, {TW_SIN, v+2 , x}, {TW_SIN, v+3 , x}, \
     {TW_SIN, v+4 , x}, {TW_SIN, v+5 , x}, {TW_SIN, v+6 , x}, {TW_SIN, v+7 , x}, \
     {TW_SIN, v+8 , x}, {TW_SIN, v+9 , x}, {TW_SIN, v+10, x}, {TW_SIN, v+11, x}, \
     {TW_SIN, v+12, x}, {TW_SIN, v+13, x}, {TW_SIN, v+14, x}, {TW_SIN, v+15, x}
#else /* !FFTW_SINGLE */
# define VTWS(v,x)                                                           \
     {TW_COS, v  , x}, {TW_COS, v+1, x}, {TW_COS, v+2, x}, {TW_COS, v+3, x}, \
     {TW_COS, v+4, x}, {TW_COS, v+5, x}, {TW_COS, v+6, x}, {TW_COS, v+7, x}, \
     {TW_SIN, v  , x}, {TW_SIN, v+1, x}, {TW_SIN, v+2, x}, {TW_SIN, v+3, x}, \
     {TW_SIN, v+4, x}, {TW_SIN, v+5, x}, {TW_SIN, v+6, x}, {TW_SIN, v+7, x}
#endif /* FFTW_SINGLE */
#define TWVLS (2 * VL)

#ifdef FFTW_SINGLE
#define VENTER() \
   svbool_t pg = svptrue_b32(); \
   svbool_t pgi = svtrn1_b32(svpfalse_b(),svptrue_b32())
#else /* !FFTW_SINGLE */
#define VENTER() \
   svbool_t pg = svptrue_b64(); \
   svbool_t pgi = svtrn1_b64(svpfalse_b(),svptrue_b64())
#endif /* FFTW_SINGLE */

#define VLEAVE() /* nothing */

#include "simd-common.h"
