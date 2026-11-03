/*
 * A251695  a(n) = (3*n+1) * (4*n+1)^(n-2) * 5^n.
 * https://oeis.org/A251695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)*(4*n+1)^(n-2)*5^n (for n >= 2), a(0) = 1, a(1) =
 *            4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 261 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Handle the first two explicit values */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;

    /* n >= 2: use the formula */
    seq_t term5   = ipow_u64_idx(5ull, n);                /* 5^n */
    uint64_t base = 4ull * n + 1;                     /* 4*n + 1 */
    seq_t termPow = ipow_u64_idx(base, n - 2u);           /* (4*n+1)^(n-2) */
    seq_t mult    = (seq_t)(3ull * n + 1);            /* 3*n + 1 */

    return mult * termPow * term5;
}
