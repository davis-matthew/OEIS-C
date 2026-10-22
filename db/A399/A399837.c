/*
 * A399837  The sum of divisors d of n such that gcud(d, n/d) is an
 *          exponentially odd number (A268335), where gcud is the greatest
 *          common unitary divisor.
 * https://oeis.org/A399837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A399837: a(n) is the sum of divisors d of n such that
 *    gcud(d, n/d) is exponentially odd.  The sequence is multiplicative
 *    and satisfies
 *        a(p^e) = σ(p^e)  if e % 4 ≠ 0
 *                = σ(p^e) – p^(e/2)  if e % 4 = 0,  e > 0
 *    where σ(p^e) = 1 + p + p^2 + … + p^e.  a(1) = 1.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Factor n into prime powers */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];

        /* Compute σ(p^e) = 1 + p + ... + p^e */
        seq_t sigma = (seq_t)1;
        seq_t pow  = (seq_t)1;
        for (unsigned j = 1; j <= e; ++j) {
            pow *= (seq_t)p;
            sigma += pow;
        }

        seq_t term = sigma;
        if ((e % 4) == 0 && e > 0) {
            /* Subtract p^(e/2) */
            seq_t sub = (seq_t)1;
            for (unsigned k = 0; k < e / 2; ++k) {
                sub *= (seq_t)p;
            }
            term -= sub;
        }

        result *= term;
    }

    return result;
}
