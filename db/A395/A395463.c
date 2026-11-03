/*
 * A395463  a(n) = n*(7^n - 1) + 1.
 * https://oeis.org/A395463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7^n - 1) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 968 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute 7^n */
    seq_t p = ipow_idx((seq_t)7, n);

    /* Compute n*(7^n - 1) + 1 */
    seq_t tmp = p - ((seq_t)1);      /* 7^n - 1 */
    tmp = ((seq_t)n) * tmp;          /* n*(7^n - 1) */
    tmp = tmp + ((seq_t)1);          /* +1 */

    return tmp;
}
