/* sigma_u64: the sum of the divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIGMA_U64_H
#define OEISC_UTILS_SIGMA_U64_H
#include <stdint.h>

/* Computes σ(n) using trial division – n <= 10⁴ in the tests. */
static seq_t sigma_u64(uint64_t n)
{
    seq_t result = 1;
    uint64_t remaining = n;

    /* factor 2 */
    if (remaining % 2 == 0) {
        unsigned exp = 0;
        while (remaining % 2 == 0) {
            remaining /= 2;
            ++exp;
        }
        seq_t part = 1;          /* 1 + 2 + 2² + … + 2^exp */
        seq_t p_pow = 1;
        for (unsigned i = 1; i <= exp; ++i) {
            p_pow *= (seq_t)2;
            part += p_pow;
        }
        result *= part;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            unsigned exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            seq_t part = 1;          /* 1 + p + p² + … + p^exp */
            seq_t p_pow = 1;
            for (unsigned i = 1; i <= exp; ++i) {
                p_pow *= (seq_t)p;
                part += p_pow;
            }
            result *= part;
        }
    }

    /* remaining prime > sqrt(n) */
    if (remaining > 1) {
        result *= (uint64_t)(remaining + 1);   /* σ(p) = 1 + p */
    }

    return result;
}

#endif
