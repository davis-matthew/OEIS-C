/*
 * A304512  a(n) = 366*2^n - 204 (n >= 1).
 * https://oeis.org/A304512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 366*2^n - 204 (n >= 1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the provided pow2 helper. */
    seq_t p = pow2(n);

    /* Evaluate the closed‑form expression. */
    return ((seq_t)366 * p) - (seq_t)204;
}
