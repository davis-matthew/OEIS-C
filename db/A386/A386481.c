/*
 * A386481  Array read by upward antidiagonals: T(k,n) = 1 (k = 0, n >= 0),
 *          T(k,n) = binomial(n,2)*k^2 + n*(k-1) + 1 (k >= 1, n >= 0).
 * https://oeis.org/A386481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k,n2) where (k,n2) is obtained by reading the array
 *            T(k,m) = 1, k=0; T(k,m) = binom(m,2)*k^2 + m*(k-1) + 1, k>=1, by
 *            upward antidiagonals (k descends on each diagonal). Indexing:
 *            diagonal s contains s+1 elements (k = s … 0). The cumulative
 *            count up to diagonal s is C(s) = s*(s+1)/2
 * Range:     u8 n <= 59, u16 n <= 796, u32 n <= 9359, u64 n <= 9359,
 *            u128 n <= 9359, big any n
 * Helpers:   _cum_div2, _diag_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_cum_div2.h"
#include "../utils/_diag_of_idx.h"

/* main sequence routine */
seq_t a(idx_t n)
{
    /* find diagonal */
    idx_t s = _diag_of_idx(n);

    /* offset t in diagonal (k goes from s to 0) */
    idx_t t = n - (idx_t)_cum_div2(s);

    /* coordinates */
    idx_t k   = s - t;
    idx_t n2 = t;

    /* T(0, *) always 1 */
    if (k == 0)
        return (seq_t)1;

    /* binom(n2,2) */
    __uint128_t binom = 0;
    if (n2 >= 2)
        binom = (__uint128_t)n2 * (n2 - 1) / 2;

    /* compute term: binom*k^2 + n2*(k-1) + 1 */
    __uint128_t kk   = (__uint128_t)k * k;
    __uint128_t term = binom * kk + (__uint128_t)n2 * (k - 1) + 1;

    return (seq_t)term;
}
