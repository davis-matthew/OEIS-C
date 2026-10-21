/*
 * A242342  a(n) = binomial(n, smallest non-divisor of n).
 * https://oeis.org/A242342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, smallest non‑divisor of n)
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 119, u64 n <= 839,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the smallest integer k ≥ 2 that does not divide n. */
    idx_t k = 2;
    while (k <= n && (n % k == 0)) {
        ++k;
    }

    /* If k > n the binomial coefficient is 0. */
    if (k > n) {
        return 0;
    }

    /* Compute the binomial coefficient nCk. */
    return binomial(n, k);
}
