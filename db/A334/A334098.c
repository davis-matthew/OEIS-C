/*
 * A334098  a(n) = A334097(n) - A331410(n), where former is the exponent of
 *          the eventual power of 2 reached, and the latter is the number of
 *          iterations needed to get there, when starting from n and using the
 *          map k -> k + k/p, where p can be any odd prime factor of k, for
 *          example, the largest.
 * https://oeis.org/A334098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A334097(n) - A331410(n)
 * Range:     u8 n <= 9906, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility from the build system: returns the largest prime factor of x. */
#include "../utils/gpf.h"

/* Compute a(p) for an odd prime p. */
static seq_t a_prime(uint64_t p)
{
    uint64_t k = p;
    unsigned iter = 0;

    /* Continue until k becomes a power of two. */
    while ((k & (k - 1)) != 0) {
        uint64_t lpf = gpf(k);          /* largest odd prime factor of k */
        k += k / lpf;                   /* k -> k + k/p */
        ++iter;
    }

    /* Determine the exponent of the final power of two. */
    seq_t exp = 0;
    while ((k & 1ULL) == 0) {
        k >>= 1;
        ++exp;
    }

    return exp - iter;
}

/* Main entry point. Computes a(n) = A334097(n) - A331410(n). */
seq_t a(idx_t n)
{
    if (n == 1) return 0;

    uint64_t temp = (uint64_t)n;
    seq_t result = 0;

    /* Account for the factor 2^e2. Each 2 contributes 1. */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++result;
    }

    /* Factor the remaining odd part by trial division. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t ap = a_prime(p);
            result += (seq_t)e * ap;
        }
    }

    /* If a prime factor greater than sqrt(original_n) remains. */
    if (temp > 1) {
        result += a_prime(temp);
    }

    return result;
}
