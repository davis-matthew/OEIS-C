/*
 * A361725  a(n) is the largest of two middle prime factors of n if the number
 *          of prime divisors counted with multiplicity (A001222(n)) is even,
 *          otherwise is the middle prime factor of n.
 * https://oeis.org/A361725
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the largest of the two middle prime factors of n when Ω(n) is even,
   otherwise the single middle prime factor.
   Ω(n) is the number of prime factors of n counted with multiplicity.
   n ≥ 2.  For n < 2 the result is 0 (behaviour below the offset does not matter).
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset behaviour irrelevant */

    /* Factor n */
    uint64_t primes[32];
    uint64_t exps  [32];
    size_t   c;                               /* number of distinct prime factors */
    factor_u64((uint64_t)n, primes, exps, &c);

    /* Count Ω(n) and collect all prime factors with multiplicity */
    size_t omega = 0;                    /* Ω(n) */
    seq_t  fac[64] = {0};
    size_t fc = 0;                        /* number of factors stored */

    for (size_t i = 0; i < c; ++i) {
        omega += (size_t)exps[i];
        for (size_t j = 0; j < (size_t)exps[i]; ++j) {
            if (fc < 64) fac[fc++] = (seq_t)primes[i];
        }
    }

    /* Pick the middle factor(s) */
    if ((omega & 1) == 0)                 /* even Ω(n) */
        return fac[fc / 2];               /* larger of the two middle factors */
    else
        return fac[(fc - 1) / 2];         /* unique middle factor */
}
