/*
 * A393442  Array read by downward antidiagonals: T(n,k) (n>=0, k>=0) is the
 *          maximum number of regions the plane can be divided into by drawing
 *          n circles and k lines.
 * https://oeis.org/A393442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(i,j) read by downward antidiagonals
 * where T(n,k) = 1 + k + 2*n*k + k*(k-1)/2 + n*(n-1),
 * except T(n,0) = n^2-n+2 if n >= 1.
 */
#include <stdint.h>

/* Integer square root for idx_t (assumed uint64_t). */
static inline idx_t isqrt_u64(idx_t v) {
    idx_t r = 0;
    /* binary search on bits (128‑bit multiplication to avoid overflow) */
    for (int i = 63; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t test = r + bit;
        if ((unsigned __int128)test * test <= v)
            r = test;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Find diagonal d such that n lies on it.  d = floor((sqrt(8n+1)-1)/2) */
    idx_t m   = 8 * n + 1;
    idx_t s   = isqrt_u64(m);
    idx_t d   = (s - 1) / 2;

    /* Previous diagonal count */
    idx_t prev = d * (d + 1) / 2;

    /* Offset within diagonal gives (i,j) */
    idx_t i = n - prev;     /* number of circles */
    idx_t j = d - i;        /* number of lines */

    if (j == 0) {
        /* Handle special case T(n,0) = n^2-n+2 for n>=1, else 1. */
        if (i == 0)
            return (seq_t)1;
        return (seq_t)( (seq_t)i * i - (seq_t)i + (seq_t)2 );
    }

    /* General formula for T(n,k) */
    seq_t sn = (seq_t)i;
    seq_t sk = (seq_t)j;
    seq_t val = 1 + sk
                + 2 * sn * sk
                + sk * (sk - 1) / 2
                + sn * (sn - 1);
    return val;
}
