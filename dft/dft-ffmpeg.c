#include "dft.h"

#define FFTW_FORWARD (-1)
#define FFTW_BACKWARD (+1)

#ifdef HAVE_FFMPEG
#include <libavcodec/avfft.h>

/* currently ffmpeg uses 16-byte variable alignment for all architectures. i.e.
 * input is expected to be alloced with av_malloc(), which is 16-byte aligned.
 */
#define FFMPEG_ALIGNMENT 16

//#define DEBUG_FFMPEG
#ifdef DEBUG_FFMPEG
#include <stdio.h>
#define D_(fmt,args...) printf(fmt,##args)
#include <assert.h>
#define A_(x) assert(x)
#else
#define D_(fmt,args...)
#define A_(x)
#endif

#define size_rank_ok(x) \
	/* 1d data */ \
	((x) == 1)
#define vector_rank_ok(x) \
	/* single vector */ \
	((x) == 0)
#define nbits_ok(x) \
	/* limits imposed by ffmpeg */ \
	((_log2(x) >= 2) && (_log2(x) <= 16))
#define input_stride_ok(x) \
	/* the usual stride for complex 32-bit float data */ \
	((x) == 2)
#define output_stride_ok(x) \
	/* the usual stride for complex 32-bit float data */ \
	((x) == 2)
#define alignment_ok(ri,ii) \
	/* XXX: paranoia - verify memory locations */ \
	(( \
		/* FFTW_FORWARD */ \
		(ii) == (ri + 1) && aligned(ri) \
	) || ( \
		/* FFTW_BACKARD */ \
		(ri) == (ii + 1) && aligned(ii) \
	))
#define io_place_ok(ri,ro,ii,io) \
	/* all ffmpeg routines are in-place */ \
	((ri) == (ro) && (ii) == (io))
#define not_null(ri,ii) \
	/* all ffmpeg routines are in-place */ \
	((ri) && (ii))

typedef struct {
	solver super;
} S;

typedef struct {
	plan_dft super;
	int n;
	int dir;
	FFTContext *s;
} P;

static int sign(const problem_dft *p);
static void unextract_reim(P *ego, const problem_dft *p);

#define POW2P(n) (((n) > 0) && (((n) & ((n) - 1)) == 0))
static int _log2(unsigned int n) {
	unsigned int m = -1;
	const unsigned int m_max = 30;
	if ( POW2P(n) ) {
		for( m = 0; m < m_max; m++ )
			if ( ((1<<m)^n) == 0 )
				break;
		if ( m == m_max ) m = -1;
	}
	return m;
}

static int aligned(void * p) {
	return !((unsigned int)p % FFMPEG_ALIGNMENT);
}

// XXX: do not rely on the same I/O pointers you were given in mkplan()
static void apply(const plan *ego_, R *ri, R *ii, R *ro, R *io)
{
	const P *ego = (const P *) ego_;
	int dir;
	R *x;

	UNUSED(ro);
	UNUSED(io);

	A_(ego && ego->s);
	A_(ri && ii);

	dir = ( ii == ri + 1 ) ? FFTW_FORWARD :
		  ((ri == ii + 1)  ? FFTW_BACKWARD : 0 );

	// ensure that we're going in the planned direction
	A_(dir && dir == ego->dir);

	x = (  dir == FFTW_FORWARD ) ? ri :
	    ( (dir == FFTW_BACKWARD) ? ii : 0 );
	A_(x);

	// make it so, number one
	av_fft_permute(ego->s,(FFTComplex *)x);
	av_fft_calc(ego->s,(FFTComplex *)x);
}

static void awake(plan *ego_, enum wakefulness wakefulness)
{
	UNUSED(ego_);
	UNUSED(wakefulness);
}

static int applicable0(const problem *p_)
{
	const problem_dft *p = (const problem_dft *) p_;

	if ( !(p &&
		   p->sz && p->vecsz &&
		   p->sz->dims && p->vecsz->dims &&
		   p->ri && p->ro && p->ii && p->io ) )
		return 0;

	return ( 1
		&& nbits_ok(p->sz->dims[0].n)
		&& size_rank_ok(p->sz->rnk)
		&& vector_rank_ok(p->vecsz->rnk)
		&& input_stride_ok(p->sz->dims[0].is)
		&& output_stride_ok(p->sz->dims[0].os)
		&& io_place_ok(p->ri,p->ro,p->ii,p->io)
		&& not_null(p->ri,p->ii)
		&& alignment_ok(p->ri,p->ii) );
}

static int applicable(const solver *ego, const problem *p_,
		const planner *plnr)
{
	UNUSED(ego);
	UNUSED(plnr);
	return applicable0(p_);
}

static void destroy(plan *ego_)
{
	P *ego = (P *) ego_;
	A_( ego && ego->s );
	if ( ego->s )
		av_fft_end(ego->s);
	ego->s = (FFTComplex *)0;
}

static void print(const plan *ego_, printer *p)
{
	const P *ego = (const P *)ego_;
	A_( ego && p );
	p->print(p, "(dft-ffmpeg-%D)",ego->n);
}

static plan *mkplan(const solver *ego, const problem *p_, planner *plnr)
{
	const problem_dft *p = (const problem_dft *) p_;
	P *pln;
	int n,m,dir;
	FFTContext *s;

	static const plan_adt padt = {
			X(dft_solve), awake, print, destroy
	};

	if ( ! applicable(ego, p_, plnr) )
		return (plan *)0;

	D_( "problem applicable!\n" );
	D_( "n = %d\n"
			"sz rnk = %d\n"
			"vc rnk = %d\n"
			"is = %d\n"
			"os = %d\n"
			"ri = %p\n"
			"ro = %p\n"
			"ii = %p\n"
			"io = %p\n"
			"alr = %d\n"
			"ali = %d\n",
			p->sz->dims[0].n,
			p->sz->rnk,
			p->vecsz->rnk,
			p->sz->dims[0].is,
			p->sz->dims[0].os,
			p->ri,
			p->ro,
			p->ii,
			p->io,
			aligned(p->ri),
			aligned(p->ii)
			);

	n = p->sz->dims[0].n;
	m = _log2(n);
	dir = ( p->ii == p->ri + 1 ) ? FFTW_FORWARD :
		  ((p->ri == p->ii + 1)  ? FFTW_BACKWARD : 0 );
	A_(dir);

	s = av_fft_init(m,(dir == FFTW_FORWARD) ? 0 : 1);
	if ( ! s )
		return (plan *)0;

	pln = MKPLAN_DFT(P, &padt, apply);

	if ( ! pln ) {
		av_fft_end(s);
		return (plan *)0;
	}

	pln->n = n;
	pln->s = s;
	pln->dir = dir;

	// FIXME: leaving zero for now
	X(ops_zero)(&pln->super.super.ops);

	return &(pln->super.super);
}

static solver *mksolver(void)
{
	static const solver_adt sadt = { PROBLEM_DFT, mkplan, 0 };
	S *slv = MKSOLVER(S, &sadt);
	A_( slv );
	return &(slv->super);
}

void X(dft_ffmpeg_register)(planner *p)
{
	REGISTER_SOLVER(p, mksolver());
}

#endif /* HAVE_FFMPEG */

/*
 * XXX: !!!IMPORTANT!!!
 *
 * DFT's (FFTW_FORWARD) use -sqrt(-1) in the exponent of the complex
 * exponential, while IDFT's (FFTW_BACKWARD) use +sqrt(-1).
 *
 * Rather than perform unnecesary complex exponentiation for direction change
 * FFTW just swaps the ri and ii input pointers
 * (see ./kernel/extrnonact-reim.c, api/api.h). The result is equivalent to
 * an IDFT using DFT code. While that is a clever solution for scalar
 * code, which does not require any special alignment, it turns possibly
 * non-explicit SIMD code (i.e. callinig ffmpeg's routines) into a train-wreck
 * of misalignment. The result: segmentation faults.
 *
 * For example, typical simd alignment is 16 bytes, where ci and ri each
 * occupy 8 bytes (4 real, 4 imag). In the "normal" case, ri is 16-byte aligned
 * while ci is 8-byte aligned. If ri and ci pointers are then swapped, the
 * result is that ri is no longer 16-byte aligned, which will break some simd
 * implementations that do not allow unaligned accesses.
 *
 * To fix this problem, its necessary to extract the direction of the
 * transform, swap the pointers again if necessary, and then call the
 * implementation's transform correctly. FFTW does not make this an easy task;
 * the 'sign' argument does not propogate past X(mkapiplan) to any
 * algorithmic code, so the only way to reliably achieve a fix is below.
 *
 * This would be one good reason to allow the 'sign' argument to propogate to
 * algorithmic code (some transforms cannot be called symmetrically).
 */
