/*
 * A323159  Greatest common divisor of product (1+(p^e)) and product (1+p),
 *          where p ranges over prime factors of n, with e corresponding
 *          exponent; a(n) = gcd(A034448(n), A048250(n)).
 * https://oeis.org/A323159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(∏(1 + p^e) , ∏(1 + p)) where n = ∏ p^e is the prime
 *            factorization of n
 * Range:     u8 n <= 137, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) computes the OEIS sequence A323159.
 * It uses the provided helper functions for factorization and gcd.
 * For each prime factor p^e of n it forms the terms (1+p^e) and (1+p),
 * multiplies them to form two products, and finally returns their gcd.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n >= 1.
     * For n == 1 the product over an empty set is 1, so the result is 1. */
    if (n == 0)
        return (seq_t)0;  /* Not required for the OEIS offset, but safe. */

    /* Allocate space for prime factors and their exponents.
     * The maximum number of distinct primes in a 64‑bit integer is 12 (2*3*5*7*11*13*17*19*23*29*31*37 > 2^64),
     * so 64 slots is plenty. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorize n.  factor_u64 fills 'primes' and 'exps' with the prime factors
     * and their exponents, and stores the count in 'cnt'. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the two products: X = ∏ (1 + p^e), Y = ∏ (1 + p). */
    seq_t prod1 = (seq_t)1;
    seq_t prod2 = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p_power = (seq_t)1;
        for (uint64_t k = 0; k < exps[i]; ++k)
            p_power *= p;          /* p^e */

        prod1 *= (p_power + (seq_t)1); /* multiply by (1 + p^e) */
        prod2 *= (p + (seq_t)1);       /* multiply by (1 + p) */
    }

    /* Return the greatest common divisor of the two products. */
    return gcd(prod1, prod2);
}
