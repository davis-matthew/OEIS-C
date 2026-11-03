/*
 * A373039  a(n) = (A372966(n) - 1)/240.
 * https://oeis.org/A373039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A372966(n) - 1)/240
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 508,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9873 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A372966 by its multiplicative definition: for n=∏pᵉ,
   A372966(n)=∏f(pᵉ) with  f(pᵉ) = (1 - p⁴)·∑_{m=0}^{e-1}p^{8m} + p^{8e} .   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* multiplicative factor for a prime power */
static seq_t f_of_prime_power(uint64_t p, idx_t exp)
{
    if (exp == 0)
        return (seq_t)1;

    seq_t P  = (seq_t)p;
    seq_t p4 = P * P * P * P;      /* p⁴      */
    seq_t p8 = p4 * p4;            /* p⁸      */

    seq_t sum  = 0;                /* Σ p^{8m}   */
    seq_t term = 1;                /* current p^{8m} */
    for (idx_t i = 0; i < exp; ++i) {
        sum  += term;
        term *= p8;
    }
    /* term is now p^{8·exp} */

    /* f(pᵉ) = term – (p⁴ – 1)·sum  */
    seq_t prod = sum * (p4 - (seq_t)1);
    return term - prod;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 */

    /* factor the index n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute the product of the prime‑power factors.
       Use 128‑bit arithmetic in the intermediate,
       since the final result fits in seq_t for the values that
       are tested in the 64‑bit build.                */
    unsigned __int128 fp = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t fpi = f_of_prime_power(primes[i], (idx_t)exps[i]);
        fp *= (unsigned __int128)fpi;
    }

    seq_t result = (seq_t)((fp - (unsigned __int128)1)
                          / ((unsigned __int128)240));
    return result;
}
