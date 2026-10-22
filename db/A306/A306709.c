/*
 * A306709  For n > 1, a(n) = gcd(A001414(n), A167344(n)) where A001414(n) is
 *          the sum of primes p dividing n (with repetition) and A167344(n) =
 *          b(n) is the totally multiplicative sequence with b(p) =
 *          (p-1)*(p+1) = p^2 - 1; a(1) = 0.
 * https://oeis.org/A306709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(A001414(n), A167344(n))
 * Range:     u8 n <= 21, u16 n <= 1915, u32 n <= 8094, u64 n <= 8094,
 *            u128 n <= 8094, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001414(n) = sum of prime factors of n (with multiplicity)          */
/* A167344(n) = product over prime powers p^e||n of (p^2-1)^e           */
#ifndef A306709_H
#define A306709_H

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                /* a(1) = 0; a(0) also 0 for safety */

    /* factorize n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;          /* sum of prime factors with multiplicity */
    seq_t prod = 1;         /* product of (p^2-1)^e for each prime power */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Accumulate sum of prime factors with multiplicity */
        sum += (seq_t)p * e;

        /* Compute (p^2-1)^e and multiply into prod */
        seq_t base = (seq_t)p * p;
        base -= 1;                 /* base = p^2 - 1 */
        for (uint64_t k = 0; k < e; ++k)
            prod *= base;
    }

    return gcd(sum, prod);
}

#endif /* A306709_H */
