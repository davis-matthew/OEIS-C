/*
 * A242446  a(n) = Sum_{k=1..n} C(n,k) * k^(2*n).
 * https://oeis.org/A242446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} C(n,k) * k^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sum = 0;
    seq_t bin = 1;            /* binomial coefficient C(n,0) */
    idx_t twon = n * 2;
    for (idx_t k = 1; k <= n; ++k)
    {
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;   /* C(n,k) from C(n,k-1) */
        seq_t power = ipow_u64_idx(k, twon);             /* k^(2*n) */
        sum += bin * power;
    }
    return sum;
}
