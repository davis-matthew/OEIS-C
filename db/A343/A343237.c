/*
 * A343237  Triangle T obtained from the array A(n, k) = (k+1)^(n+1) -
 *          k^(n+1), n, k >= 0, by reading antidiagonals upwards.
 * https://oeis.org/A343237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (k+1)^(n-k+1) - k^(n-k+1), where k = n - d, with d = the
 *            smallest integer such that (d+1)(d+2)/2 > n (i.e. n lies on the
 *            anti‑diagonal number d)
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal d: the smallest non‑negative integer with
       (d+1)(d+2)/2 > n, so that the n-th term lies on that diagonal. */
    idx_t lo = 0, hi = 1;
    while (((__int128)(hi + 1) * (hi + 2)) / 2 <= n)
        hi <<= 1;                /* increase hi until we pass n */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((__int128)(mid + 1) * (mid + 2)) / 2 > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;                 /* diagonal number */

    /* Sum of elements in all previous diagonals */
    idx_t s_prev = (idx_t)((( __int128)d * (d + 1)) / 2);
    idx_t pos = n - s_prev;       /* position inside diagonal (0 … d) */

    idx_t n_val = d - pos;        /* n in A(n-m, m) */
    idx_t k_val = pos;            /* m in A(n-m, m) */
    idx_t exp   = n_val + 1;      /* (n-m)+1 */

    seq_t term1 = pow_uint((uint64_t)(k_val + 1), exp);
    seq_t term2 = pow_uint((uint64_t)k_val, exp);
    return term1 - term2;
}
