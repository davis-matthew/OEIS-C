/*
 * A275468  Least k such that d(k*n^2) = d(n*k^2) where d = A000005.
 * https://oeis.org/A275468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k such that d(k*n^2) = d(n*k^2)
 * Range:     u8 n <= 255, u16 n <= 9401, u32 n <= 9401, u64 n <= 9401,
 *            u128 n <= 9401, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses repeated factorisation of n and k.   */
/* The divisor function d(m) is computed multiplicatively from prime exponents. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined for offset 1, but protect */
        return (seq_t)0;

    /* factorise n once */
    uint64_t primesN[16];
    uint64_t expsN[16];
    size_t   cntN;
    factor_u64((uint64_t)n, primesN, expsN, &cntN);

    uint64_t expN2[16];
    for (size_t i = 0; i < cntN; ++i)
        expN2[i] = expsN[i] * 2ULL;

    /* search k from 1 up to n (k = n always satisfies the condition) */
    uint64_t limit = (uint64_t)n;
    for (uint64_t k = 1; k <= limit; ++k)
    {
        /* factorise k */
        uint64_t primesK[16];
        uint64_t expsK[16];
        size_t   cntK;
        factor_u64(k, primesK, expsK, &cntK);

        seq_t d1 = 1;
        seq_t d2 = 1;

        size_t i = 0, j = 0;
        while (i < cntN || j < cntK)
        {
            uint64_t  p;
            uint64_t  eN = 0, eN2 = 0, eK = 0;

            if (i < cntN && (j == cntK || primesN[i] < primesK[j]))
            {
                p   = primesN[i];
                eN  = expsN[i];
                eN2 = expN2[i];
                ++i;
            }
            else if (j < cntK && (i == cntN || primesK[j] < primesN[i]))
            {
                p  = primesK[j];
                eK = expsK[j];
                ++j;
            }
            else /* same prime in both factorizations */
            {
                p   = primesN[i];
                eN  = expsN[i];
                eN2 = expN2[i];
                eK  = expsK[j];
                ++i;
                ++j;
            }

            uint64_t exp_m1 = eN2 + eK;        /* exponent in k * n^2   */
            uint64_t exp_m2 = eN + (eK << 1);  /* exponent in n * k^2   */

            d1 *= (seq_t)(exp_m1 + 1);
            d2 *= (seq_t)(exp_m2 + 1);
        }

        if (d1 == d2)
            return (seq_t)k; /* found minimal k */
    }

    /* Fallback: k = n always works */
    return (seq_t)n;
}
