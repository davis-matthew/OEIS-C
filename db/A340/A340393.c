/*
 * A340393  Treat the prime factors of n in ascending order as digits of a
 *          number in base "greatest prime factor + 1" and convert this number
 *          back to a decimal number.
 * https://oeis.org/A340393
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 207, u32 n <= 2335, u64 n <= 9334,
 *            u128 n <= 9334, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A340393:
   Treat the prime factors of n in ascending order as digits of a number
   in base (greatest prime factor + 1) and return that number in decimal. */

#include "../utils/factor_u64.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* offset 2 – behaviour for n < 2 is irrelevant */
        return 0;

    /* Factorise n and collect prime factors with multiplicity. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t factors[256];   /* holds the factors in ascending order */
    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = 0; j < exps[i]; ++j) {
            factors[k++] = primes[i];
        }
    }

    /* Base is greatest prime factor + 1. */
    seq_t base = (seq_t)(factors[k - 1] + 1);

    /* Convert the digit list to a decimal number. */
    seq_t result = 0;
    for (size_t i = 0; i < k; ++i)
        result = result * base + (seq_t)factors[i];

    return result;
}
