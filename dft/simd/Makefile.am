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
SUBDIRS = common sse2 avx avx-128-fma avx2 avx2-128 avx512 kcvi altivec vsx neon sve generic-simd128 generic-simd256
EXTRA_DIST = n1b.h n1f.h n2b.h n2f.h n2s.h q1b.h q1f.h t1b.h t1bu.h	\
t1f.h t1fu.h t2b.h t2f.h t3b.h t3f.h ts.h codlist.mk simd.mk
