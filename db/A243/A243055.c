/*
 * A243055  Difference between the indices of the smallest and the largest
 *          prime dividing n: If n = p_i * ... * p_k, where p_i <= ... <= p_k,
 *          where p_h = A000040(h), then a(n) = (k-i), a(1) = 0 by convention.
 * https://oeis.org/A243055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between indices of smallest and largest prime
 *            factors of n. a(n) = A061395(n) - A055396(n) where A061395(n) =
 *            index of the largest prime dividing n, A055396(n) = index of the
 *            smallest prime dividing n
 * Range:     u8 n <= 3241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9303 (2026-11-01)
 * Helpers:   largest_prime_factor, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/prime_pi.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 by convention */

    /* Smallest and largest prime factors (values) */
    uint64_t sp = lpf((uint64_t)n);
    uint64_t gp = largest_prime_factor((uint64_t)n);

    /* If n is prime the difference is zero */
    if (sp == gp) return (seq_t)0;

    /* Convert prime values to their indices via the prime‑counting function */
    seq_t i = prime_pi((idx_t)sp);    /* index of smallest prime factor */
    seq_t k = prime_pi((idx_t)gp);    /* index of largest prime factor */

    return k - i;                       /* difference of indices */
}
