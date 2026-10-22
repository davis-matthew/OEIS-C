/*
 * A345013  Triangle read by rows, related to clusters of type D.
 * https://oeis.org/A345013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = T(r, k) where r = ceil((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r-1)/2 - 1 and T(n,k) = (n-k)*C(n,k)*C(2*n-k, n-1)/n
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* find the row r such that r*(r+1)/2 >= n */
static idx_t row_of(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until the triangular number exceeds n */
    while ((__int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;

    /* binary search on the interval [lo, hi] */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* OEIS A345013: triangle read by rows (clusters of type D). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 safely */
        return 0;

    idx_t r   = row_of(n);                     /* row index (1‑based) */
    idx_t prev = r * (r - 1) / 2;              /* entries before this row */
    idx_t k   = n - prev - 1;                  /* column index (0‑based) */

    seq_t binom1 = binomial(r, k);             /* C(r, k) */
    seq_t binom2 = binomial(2 * r - k, r - 1); /* C(2r - k, r-1) */

    seq_t t = binom1 * binom2;                 /* product of binomials   */
    t = t * (seq_t)(r - k);                    /* multiply by (r-k)      */
    t = t / (seq_t)r;                          /* divide by r, integer   */
    return t;
}
