/*
 * Copyright (c) 2003, 2007 Matteo Frigo
 * Copyright (c) 2003, 2007 Massachusetts Institute of Technology
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "codelet-rdft.h"
#include "hc2cf.h"

static int okp(const R *Rp, const R *Ip, const R *Rm, const R *Im, 
	       INT rs, INT mb, INT me, INT ms, 
	       const planner *plnr)
{
     UNUSED(Rp); UNUSED(Ip); UNUSED(Rm); UNUSED(Im);
     UNUSED(rs); UNUSED(mb); UNUSED(me); UNUSED(ms); UNUSED(plnr);

     return 1;
}

const hc2c_genus GENUS = { okp, R2HC, 1 };

#undef GENUS
#include "hc2cb.h"

const hc2c_genus GENUS = { okp, HC2R, 1 };
