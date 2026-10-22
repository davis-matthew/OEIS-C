/*
 * A359405  Number of unordered pairs of self-avoiding paths with nodes that
 *          cover all vertices of a convex n-gon; one-node paths are allowed.
 * https://oeis.org/A359405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*2^(n-6)*(2^(n-3)+3) for n >= 3; a(1) = 0, a(2) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;

    /* Handle n = 3,4,5 with a small denominator 2^(6-n) to avoid negative shifts */
    if (n <= 5) {
        seq_t num = nn * n1;
        seq_t term = pow2(n - 3) + (seq_t)3;
        num = num * term;
        seq_t denom = pow2(6 - n);   /* 2,4,8 for n=5,4,3 respectively */
        return num / denom;
    }

    /* General case: n >= 6 */
    seq_t pow_n6 = pow2(n - 6);
    seq_t pow_n3 = pow2(n - 3);
    seq_t result = nn * n1;
    result = result * pow_n6;
    result = result * (pow_n3 + (seq_t)3);
    return result;
}
