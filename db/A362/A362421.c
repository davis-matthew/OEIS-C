/*
 * A362421  Number of nonisomorphic vector spaces consisting of n elements.
 * https://oeis.org/A362421
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ₀(k) if n = pᵏ is a prime power, 0 otherwise
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t m = (uint64_t)n;

    /* Find the smallest prime factor of n */
    uint64_t p = 0;
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0) {
        /* n is prime → pᵏ with k = 1 */
        return 1;
    }

    /* Count the exponent k of the prime factor p in n */
    uint64_t k = 0;
    while (m % p == 0) {
        m /= p;
        ++k;
    }

    /* If any other prime factor remains, n is not a prime power */
    if (m != 1)
        return 0;

    /* Count the positive divisors of k (τ(k)) */
    seq_t cnt = 0;
    for (uint64_t d = 1; d <= k / d; ++d) {
        if (k % d == 0) {
            ++cnt;
            if (k / d != d)    /* avoid double‑counting the square root */
                ++cnt;
        }
    }
    return cnt;
}
