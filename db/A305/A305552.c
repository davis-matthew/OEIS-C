/*
 * A305552  Number of uniform normal multiset partitions of weight n.
 * https://oeis.org/A305552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(2^(n/d - 1) + d - 1, d)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute binomial(N, k) for possibly very large N, with k small (≤n). */
static seq_t binom_big(seq_t N, idx_t K)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t numerator = N - (seq_t)(K - i);
        result = result * numerator / i;
    }
    return result;
}

/* Compute the term for a given divisor d of n. */
static seq_t term_for(idx_t n, idx_t d)
{
    idx_t e = n / d - 1;
    seq_t p = pow2(e);
    seq_t N = p + (seq_t)d;
    N -= 1;
    return binom_big(N, d);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;    /* a(0) = 1 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            sum += term_for(n, d1);
            idx_t d2 = n / i;
            if (d1 != d2) {
                sum += term_for(n, d2);
            }
        }
    }
    return sum;
}
