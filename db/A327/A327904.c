/*
 * A327904  Number of labeled simple graphs with vertices {1..n} such that
 *          every edge has a different sum.
 * https://oeis.org/A327904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..2*n+1} ceil(k/4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t limit = 2 * n + 1;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t f = ((seq_t)k + 3) / 4;  /* ceil(k/4) */
        prod *= f;
    }
    return prod;
}
