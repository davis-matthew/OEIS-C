/*
 * A368492  a(n) = Sum_{k=0..n} k^(n+k).
 * https://oeis.org/A368492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^(n+k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Compute a(n) = Σ_{k=0}^{n} k^(n+k) */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            /* 0^0 is defined as 1, otherwise 0 */
            term = ((idx_t)n == 0) ? 1 : 0;
        } else {
            /* exponent = n + i */
            seq_t exp = (seq_t)n + (seq_t)i;
            term = big_pow((seq_t)i, exp);
        }
        sum += term;
    }
    return sum;
}
