#===============================================================================
# Copyright (c) 2020 FUJITSU LIMITED
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#===============================================================================

AM_CPPFLAGS = -I $(top_srcdir)
noinst_LTLIBRARIES = libsimd_support.la

libsimd_support_la_SOURCES = taint.c simd-common.h \
x86-cpuid.h amd64-cpuid.h \
simd-sse2.h sse2.c \
avx.c simd-avx.h \
avx-128-fma.c simd-avx-128-fma.h \
avx2.c simd-avx2.h simd-avx2-128.h \
avx512.c simd-avx512.h \
kcvi.c simd-kcvi.h \
altivec.c simd-altivec.h vsx.c simd-vsx.h \
neon.c simd-neon.h sve.c simd-sve.h \
simd-generic128.h simd-generic256.h

