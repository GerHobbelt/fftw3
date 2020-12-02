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
SUBDIRS = scalar simd

noinst_LTLIBRARIES = librdft.la

RDFT2 = buffered2.c direct2.c nop2.c rank0-rdft2.c rank-geq2-rdft2.c	\
plan2.c problem2.c solve2.c vrank-geq1-rdft2.c rdft2-rdft.c		\
rdft2-tensor-max-index.c rdft2-inplace-strides.c rdft2-strides.c	\
khc2c.c ct-hc2c.h ct-hc2c.c ct-hc2c-direct.c

librdft_la_SOURCES = hc2hc.h hc2hc.c dft-r2hc.c dht-r2hc.c dht-rader.c	\
buffered.c codelet-rdft.h conf.c direct-r2r.c direct-r2c.c generic.c	\
hc2hc-direct.c hc2hc-generic.c khc2hc.c kr2c.c kr2r.c indirect.c nop.c	\
plan.c problem.c rank0.c rank-geq2.c rdft.h rdft-dht.c solve.c		\
vrank-geq1.c vrank3-transpose.c $(RDFT2)
