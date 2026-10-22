/*
 * A365541  Irregular triangle read by rows where T(n,k) is the number of
 *          subsets of {1..n} containing two distinct elements summing to k =
 *          3..2n-1.
 * https://oeis.org/A365541
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^r - 2^(r-2m) * 3^m, where r = floor(sqrt(n-2)) + 2, k =
 *            3 + (n-2 - (r-2)^2), m = max(0, floor((k-1)/2) - max(1,k-r) + 1)
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  // undefined, return 0

    idx_t t = n - 2;
    idx_t sqrt_t = isqrt_u64(t);
    idx_t r = sqrt_t + 2;                     // row number
    idx_t p = t - sqrt_t * sqrt_t;            // position inside row, 0‑based
    idx_t k = 3 + p;                          // target sum

    /* compute number of valid pairs m */
    idx_t L = (k > r) ? (k - r) : 1;          // max(1, k-r)
    idx_t R = (k - 1) / 2;                    // floor((k-1)/2)
    idx_t m = (L <= R) ? (R - L + 1) : 0;

    seq_t pow2_n = seq_pow((seq_t)2, r);
    if (m == 0)
        return (seq_t)0;  // no subset contains two elements summing to k

    idx_t exp2 = r - 2 * m;                    // r-2m, guaranteed non‑negative
    seq_t pow2_n_minus_2m = seq_pow((seq_t)2, exp2);
    seq_t pow3_m = seq_pow((seq_t)3, m);

    seq_t avoid = pow3_m * pow2_n_minus_2m;
    return pow2_n - avoid;
}
