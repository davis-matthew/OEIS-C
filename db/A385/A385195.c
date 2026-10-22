/*
 * A385195  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is either 1 or 2.
 * https://oeis.org/A385195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385195:
   a(n) = ∏_{p^e‖n} b(p^e)
   where b(2^1)=2, and for every other prime power b(p^e)=p^e‑1.
   This multiplicative function yields the number of k∈[1,n]
   whose greatest unitary divisor with n is 1 or 2. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64(n,pr,ex,&cnt) */

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;

    /* factor_u64 returns the prime factors and their exponents of n.
       Since n fits in 64 bits, at most 64 distinct primes are possible
       (actually far fewer).  We use fixed‑size stack arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t factor;

        if (p == 2u && e == 1u) {
            factor = (seq_t)2;
        } else {
            /* compute p^e in seq_t */
            seq_t power = (seq_t)1;
            for (idx_t j = 0; j < (idx_t)e; ++j) {
                power *= (seq_t)p;
            }
            factor = power - (seq_t)1;
        }

        result *= factor;
    }

    return result;
}
