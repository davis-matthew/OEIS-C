/*
 * A308504  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} d^(n+k).
 * https://oeis.org/A308504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma_i(j) * where i is the unique integer with T_{i-1} <
 *            n ≤ T_i (T_k = k(k+1)/2), * and j = n - T_{i-1}. sigma_i(j) =
 *            ∑_{d|j} d^i
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* Find i such that i*(i+1)/2 >= n */
    idx_t i = 1;
    while (i * (i + 1) / 2 < n) ++i;

    /* Compute j = n - T_{i-1} */
    idx_t prev_tri = (i - 1) * i / 2;
    idx_t j = n - prev_tri;        /* 1 ≤ j ≤ i */

    seq_t sum = 0;
    idx_t d;

    for (d = 1; d * d <= j; ++d) {
        if (j % d == 0) {
            sum += big_pow((seq_t)d, (seq_t)i);
            idx_t q = j / d;
            if (q != d)
                sum += big_pow((seq_t)q, (seq_t)i);
        }
    }
    return sum;
}
