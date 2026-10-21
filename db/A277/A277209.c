/*
 * A277209  Partial sums of repdigit numbers (A010785).
 * https://oeis.org/A277209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the first n repdigit numbers in base 10
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* first 9 terms are just the triangular numbers */
    if (n <= 9) {
        seq_t nn = (seq_t)n;
        return nn * (nn + 1) / 2;
    }

    /* find the length L of the nth repdigit:
       9 numbers per length, so L = ceil(n/9) */
    idx_t L = (n + 8) / 9;               /* L >= 2 */
    idx_t total_before = 9 * (L - 1);    /* repdigits up to length L-1 */
    idx_t k_rel = n - total_before;     /* 1..9 within current length */

    /* powers of 10 needed */
    seq_t pow10L        = pow10(L);       /* 10^L */
    seq_t pow10Lminus1  = pow10(L - 1);   /* 10^(L-1) */

    /* sum of the first k_rel repdigits of length L
       rep(L,d) = d*(10^L-1)/9, sum_{d=1..k_rel} d = k_rel*(k_rel+1)/2  */
    seq_t t1 = (pow10L - 1) / 9;
    seq_t t2 = (seq_t)k_rel * ((seq_t)k_rel + 1) / 2;
    seq_t group_partial = t1 * t2;

    /* sum of all complete groups of length < L.
       For each length l: sum_{d=1..9} rep(l,d) = 5*(10^l-1)  */
    seq_t sum_geom = 10 * ((pow10Lminus1 - 1) / 9);   /* Σ_{l=1}^{L-1}10^l */
    seq_t geom_minus_ones = sum_geom - (L - 1);
    seq_t total_full = 5 * geom_minus_ones;

    return total_full + group_partial;
}
