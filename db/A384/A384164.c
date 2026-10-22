/*
 * A384164  a(n) = Product_{k=0..n-1} (3*n+k).
 * https://oeis.org/A384164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (3*n + k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base3   = (seq_t)3 * (seq_t)n;
    for (idx_t i = 0; i < n; ++i) {
        seq_t factor = base3 + (seq_t)i;
        result = seq_mul(result, factor);
    }
    return result;
}
