/*
 * A253192  Number of ways to place nonintersecting diagonals in convex
 *          (n+3)-gon so as to create exactly one triangle.
 * https://oeis.org/A253192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} C(n+k+2, k) * C(n-k-1, k-1) for n >
 *            0, a(0)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    idx_t limit = n / 2;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t term1 = binomial(n + k + 2, k);
        seq_t term2 = binomial(n - k - 1, k - 1);
        result += term1 * term2;
    }
    return result;
}
