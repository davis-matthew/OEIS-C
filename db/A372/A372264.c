/*
 * A372264  a(n) = n! - n^2 + 2n - 1.
 * https://oeis.org/A372264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! - n^2 + 2n - 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t nseq = (seq_t)n;
    /* a(n) = n! - n^2 + 2n - 1 */
    seq_t val = fact - nseq * nseq + 2 * nseq - 1;
    return val;
}
