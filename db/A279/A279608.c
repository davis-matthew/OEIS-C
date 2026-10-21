/*
 * A279608  Beatty sequence for e/(e - 2); i.e., a(n) = floor(n*e/(e - 2)).
 * https://oeis.org/A279608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*e/(e - 2)) (Beatty sequence for e/(e-2))
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 _D = 0;      /* D = 30!         */
static unsigned __int128 _N = 0;      /* N = sum_{k=0}^{30} D/k!  */
static unsigned __int128 _P = 0;      /* P = N - 2*D, denominator of (e-2) */
static int _init_done = 0;

/* One-time initialization of constants */
static void _init_constants(void)
{
    if (_init_done) return;
    const int M = 30;
    unsigned __int128 fact[ M + 1 ];
    fact[0] = 1;
    for (int i = 1; i <= M; ++i)
        fact[i] = fact[i-1] * (unsigned __int128)i;

    _D = fact[M];                 /* D = M!  */

    unsigned __int128 N = 0;
    for (int k = 0; k <= M; ++k)
        N += _D / fact[k];       /* N = Σ D/k!  */

    _N = N;
    _P = N - 2*_D;                /* positive denominator for (e-2)  */
    _init_done = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out-of-range, unused but harmless */

    _init_constants();

    /* Compute floor( 2*n*D / P ) using 128-bit arithmetic */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 num  = nn * (unsigned __int128)2 * _D;
    unsigned __int128 fval = num / _P;   /* floor value */

    seq_t floor_val = (seq_t)fval;
    seq_t result    = (seq_t)n + floor_val;
    return result;
}
