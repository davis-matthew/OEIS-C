/*
 * A375840  a(n) = Product_{k=0..n} (k^3 + n).
 * https://oeis.org/A375840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (k^3 + n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = ((seq_t)k * k * k) + (seq_t)n;
        prod *= term;
    }
    return prod;
}
