/*
 * A257888  Number of nonintersecting (or self-avoiding) rook paths of length
 *          2n+2 joining opposite corners of an n X n grid.
 * https://oeis.org/A257888
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 2*(n-1)*binomial(2*n-2, n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t result = 2 * (seq_t)(n - 1);
    result *= binomial(2 * n - 2, n - 3);
    return result;
}
