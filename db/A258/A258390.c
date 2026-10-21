/*
 * A258390  Number of 2n-length strings of balanced parentheses of exactly 2
 *          different types that are introduced in ascending order.
 * https://oeis.org/A258390
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n-1)-1) * binomial(2*n, n) / (n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t pow = pow2(n - 1);
    pow -= 1;
    seq_t bin = binomial(2 * n, n);
    bin /= (n + 1);
    return pow * bin;
}
