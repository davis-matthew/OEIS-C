/*
 * A271870  Convolution of nonzero hexagonal numbers (A000384) with
 *          themselves.
 * https://oeis.org/A271870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(n + 3)*(4*n^2 + 6*n + 5)/30
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline void reduce_factor(seq_t *factor, seq_t *divisor)
{
    seq_t g = gcd(*factor, *divisor);
    *factor /= g;
    *divisor /= g;
}

seq_t a(idx_t n)
{
    seq_t A = n + 1;
    seq_t B = n + 2;
    seq_t C = n + 3;
    seq_t D = (seq_t)4 * (seq_t)n * (seq_t)n + (seq_t)6 * (seq_t)n + 5;

    seq_t div = 30;
    reduce_factor(&A, &div);
    reduce_factor(&B, &div);
    reduce_factor(&C, &div);
    reduce_factor(&D, &div);

    /* After reducing, div should be 1; it is guaranteed by the definition. */
    seq_t result = A * B * C * D;
    return result;
}
