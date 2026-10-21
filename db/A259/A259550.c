/*
 * A259550  a(n) = C(5*n-1,2*n)/3, n > 0, a(0) = 1.
 * https://oeis.org/A259550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5*n-1, 2*n)/3 (OEIS A259550)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the binomial coefficient C(N, K) with exact arithmetic,
   avoiding intermediate overflow by reducing at each step. */
static seq_t binom(idx_t N, idx_t K) {
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i); /* numerator term */
        seq_t den = (seq_t)i;           /* denominator term */

        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;

        /* den should now be 1 */
        result *= num;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t top = 5 * n - 1;
    idx_t bot = 2 * n;

    seq_t val = binom(top, bot);
    return val / (seq_t)3;
}
