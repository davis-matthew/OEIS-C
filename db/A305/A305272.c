/*
 * A305272  a(n) = 836*2^n - 676.
 * https://oeis.org/A305272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 836*2^n - 676
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 160  (first term in OEIS) */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    seq_t two_n = pow2(n);                     /* 2^n  (arbitrary‑precision) */
    seq_t res   = (seq_t)836 * two_n - (seq_t)676;
    return res;
}
