/*
 * A331839  a(n) = (4^(n + 1) - 2)*(2*n)!.
 * https://oeis.org/A331839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(n+1) - 2)*(2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* compute (2*n)! */
    idx_t double_n = n + n;          /* 2*n as idx_t */
    seq_t fact = factorial(double_n);

    /* compute 4^(n+1) - 2 */
    seq_t pow4 = ipow_idx((seq_t)4, n + 1);
    seq_t adjust = pow4 - (seq_t)2;

    /* multiply the two parts */
    return fact * adjust;
}
