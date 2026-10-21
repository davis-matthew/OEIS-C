/*
 * A289183  a(n) is the greatest m such that 2*H(n) > H(m), where H(n) is the
 *          n-th harmonic number.
 * https://oeis.org/A289183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest integer m such that 2 * H(n) > H(m), where
 *            H(k) = 1 + 1/2 + … + 1/k is the k‑th harmonic number
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ------------------------------------------------------------ */
/*  Harmonic number table (exact as doubles, up to the largest n
    requested so far).  The table is indexed from 0 to _max_harm,
    with H(0) = 0.  The table grows only when a larger n is
    encountered.  Subsequent calls reuse the cached values.     */
/* ------------------------------------------------------------ */
static idx_t   _max_harm = 0;     /* largest index for which values are stored */
static double *_harmonic = NULL;  /* array: _harmonic[i] = H(i) */

static void _ensure_harmonic(idx_t n)
{
    /* nothing to do if we already have enough entries */
    if (n <= _max_harm) return;

    /* determine a new capacity (power of two, at least n+1) */
    size_t new_cap = (_max_harm ? (_max_harm << 1) : 16);
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    double *tmp = (double *)realloc(_harmonic, new_cap * sizeof(double));
    if (!tmp) {
        /* allocation failed – abort program (build system will treat it as error) */
        abort();
    }
    _harmonic = tmp;

    /* compute new harmonic values */
    for (idx_t i = _max_harm + 1; i <= n; ++i)
        _harmonic[i] = _harmonic[i-1] + 1.0 / (double)i;

    _max_harm = n;
}

/* ------------------------------------------------------------ */
/*  Asymptotic estimate of H(m) for m beyond the table.  The
    expansion used is accurate enough for all  m  that may appear
    in the binary search (error < 1/(12 m²)).                        */
/* ------------------------------------------------------------ */
static inline double _harmonic_estimate(idx_t m)
{
    if (m == 0) return 0.0;
    if (m <= _max_harm) return _harmonic[(size_t)m];

    static const double gamma = 0.57721566490153286060651209;  /* Euler–Mascheroni */
    double x = (double)m;
    double invx = 1.0 / x;
    double invx2 = invx * invx;
    double invx4 = invx2 * invx2;
    return log(x) + gamma + 0.5 * invx - invx2 / 12.0 + invx4 / 120.0;
}

/* ------------------------------------------------------------ */
/*  Main sequence function.  It returns seq_t values, converting
    the final count (an idx_t) to seq_t at the end.               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure we have exact harmonic numbers up to n for the target. */
    _ensure_harmonic(n);
    double target = 2.0 * _harmonic[(size_t)n];     /* 2 * H(n) */

    /* Find an upper bound for the desired m by exponential search.  */
    idx_t lo = 1, hi = 1;
    while (_harmonic_estimate(hi) < target) {
        /* Prevent catastrophic overflow of hi: stop if it would wrap. */
        if (hi > (~(idx_t)0) >> 1) break;
        hi <<= 1;
    }

    /* Binary search for the smallest m with H(m) ≥ target.  */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (_harmonic_estimate(mid) < target)
            lo = mid;
        else
            hi = mid;
    }

    /* lo is the greatest m such that H(m) < 2 * H(n).  */
    return (seq_t)lo;
}
