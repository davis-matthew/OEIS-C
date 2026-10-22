/*
 * A390101  Irregular triangle read by rows: T(n, k) is the minimum positive x
 *          such that (n mod x) = k.
 * https://oeis.org/A390101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 9208, u16 n <= 9208, u32 n <= 9208, u64 n <= 9208,
 *            u128 n <= 9208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Row‑wise read of the irregular triangle defined by
   T(n, k) = the smallest positive integer x such that n mod x = k.
   Each row n contains ceil(n/2) elements (k = 0…⌊(n−1)/2⌋),
   and a(n) lists all T(n, k) in row‑major order starting with n=1. */

#include <stdint.h>   /* for uint64_t if not already defined */

/* Helper: cumulative number of terms up to row r inclusive */
static inline __int128 cum_terms(idx_t r)
{
    idx_t t = (r + 1) / 2;          /* ceil(r/2) */
    if ((r & 1) == 0)               /* r even: r = 2t */
        return (__int128)t * (t + 1);
    else
        return (__int128)t * t;     /* r odd : r = 2t-1 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* not needed for the OEIS offset but harmless */
        return (seq_t)0;

    /* Find row r such that cum_terms(r-1) < n <= cum_terms(r) */
    idx_t lo = 1, hi = 1;
    while (cum_terms(hi) < n) {
        if (hi >= ((idx_t)-1) / 2) {  /* avoid overflow */
            hi = (idx_t)-1;
            break;
        }
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum_terms(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Offset of the term inside its row */
    idx_t prev = (idx_t)cum_terms(r - 1);   /* terms before row r */
    idx_t k = n - prev - 1;                /* zero‑based column */

    /* Compute T(r, k) */
    if (k == 0)
        return (seq_t)1;

    idx_t m = r - k;                       /* positive integer > k */
    idx_t d;
    for (d = k + 1; d <= m; ++d)           /* search for the smallest divisor > k */
    {
        if (m % d == 0)
            break;
    }
    return (seq_t)d;
}
