/*
 * A316441  a(n) = Sum (-1)^k where the sum is over all factorizations of n
 *          into factors > 1 and k is the number of factors.
 * https://oeis.org/A316441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{n = ∏ f_i, f_i > 1} (-1)^{#factors}
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 95 terms through n = 9972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all multiplicative factorizations
   in non‑decreasing order and sums the signs of each factorization. */

#include <stdint.h>

/* Recursive helper that returns the contribution of all
   factorizations of `n` with the smallest allowed factor `min_factor`.
   `k` is the number of factors chosen so far. */
static seq_t factor_sum(uint64_t n, uint64_t min_factor, unsigned int k)
{
    seq_t sum = 0;
    for (uint64_t i = min_factor; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += factor_sum(n / i, i, k + 1);
        }
    }
    /* Terminate this branch by taking `n` itself as a factor. */
    sum += ((k + 1) & 1) ? (seq_t)-1 : (seq_t)1;
    return sum;
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) = 1 */
    if (n == 1) {
        return 1;
    }
    return factor_sum((uint64_t)n, 2, 0);
}
