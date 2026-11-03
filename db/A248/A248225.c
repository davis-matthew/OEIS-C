/*
 * A248225  a(n) = 6^n - 3^n.
 * https://oeis.org/A248225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided exponentiation helper. */
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p6 = ipow_idx((seq_t)6, n);
    seq_t p3 = ipow_idx((seq_t)3, n);
    return p6 - p3;
}
