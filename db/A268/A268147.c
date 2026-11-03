/*
 * A268147  A double binomial sum involving absolute values.
 * https://oeis.org/A268147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 794 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function pow_uint from the utils library. */
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 16^n (with arbitrary precision if needed). */
    seq_t p = ipow_u64_idx(16, n);
    /* Multiply by n.  Conversion to seq_t is safe since n fits in idx_t. */
    p *= (seq_t)n;
    return p;
}
