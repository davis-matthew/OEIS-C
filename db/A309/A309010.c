/*
 * A309010  Square array A(n, k) = Sum_{j=0..n} binomial(n,j)^k, n >= 0, k >=
 *          0, read by antidiagonals.
 * https://oeis.org/A309010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read along the antidiagonals of * the square
 *            array A(i,k) = Σ_{t=0..i} binomial(i,t)^k. * For a given index
 *            n, find the unique non‑negative integers * (i,k) with i+ k = s
 *            such that n lies on diagonal s, then * a(n) = A(i,k). The code
 *            computes that value directly
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 110, u64 n <= 180,
 *            u128 n <= 336, big any n
 * Helpers:   power_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/power_small.h"

/* Find the diagonal s such that C(s-1) <= n < C(s), where C(t) = (t+1)(t+2)/2.
   This uses a binary search on 128‑bit integers to avoid overflow. */
static idx_t diagonal_of(idx_t n) {
    idx_t lo = 0, hi = 1;
    /* Expand hi until C(hi) > n */
    while ( ( (__uint128_t)(hi+1) * (hi+2) / 2 ) <= n ) {
        hi <<= 1;                      /* double hi */
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t val = ( (__uint128_t)(mid+1) * (mid+2) ) / 2;
        if (val > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Determine which diagonal (i + k = s) the index n lies on. */
    idx_t s = diagonal_of(n);

    /* Number of terms before this diagonal: C(s-1) = s*(s+1)/2. */
    __uint128_t prev = ( __uint128_t )s * (s + 1) / 2;
    idx_t offset = n - prev;          /* offset within the diagonal (0 <= offset <= s) */

    idx_t i = offset;                 /* row index */
    idx_t k = s - offset;             /* column index */

    /* Compute A(i,k) = Σ_{t=0..i} binomial(i,t)^k */
    seq_t sum = 0;
    seq_t cur_binom = 1;              /* binomial(i,0) = 1 */

    for (idx_t t = 0; t <= i; ++t) {
        /* pow = cur_binom^k */
        if (k == 0) {
            sum += 1;                 /* binomial^0 = 1 */
        } else {
            seq_t pow_val = power_small(cur_binom, k);
            sum += pow_val;
        }

        /* Update binomial coefficient to binomial(i, t+1) */
        if (t < i) {
            cur_binom = cur_binom * (i - t) / (t + 1);
        }
    }

    return sum;
}
