/*
 * A264724  a(n) = n^2 + phi(n).
 * https://oeis.org/A264724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + phi(n)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

/* External linkage, no static */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* convert index to seq_t once */
    seq_t sq = sn * sn;         /* n^2 in seq_t */
    seq_t ph = phi(n);          /* Euler totient of n */
    return sq + ph;             /* a(n) */
}
