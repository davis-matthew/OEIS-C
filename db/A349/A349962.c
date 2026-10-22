/*
 * A349962  a(n) = Sum_{k=0..n} (2*k)^k.
 * https://oeis.org/A349962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ{k=0..n} (2*k)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* Computes the OEIS sequence A349962: a(n) = Σ_{k=0}^n (2*k)^k. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term;
        if (k == 0) {
            term = 1;                      /* 0^0 is defined as 1 */
        } else {
            term = seq_pow((seq_t)(2 * k), k);
        }
        sum += term;
    }
    return sum;
}
