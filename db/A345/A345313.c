/*
 * A345313  a(n) = Sum_{p|n, p prime} (n/p)^lcm(p,n/p).
 * https://oeis.org/A345313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p|n, p prime} (n/p)^lcm(p,n/p) (OEIS A345313)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */

/* Compute a(n) for the OEIS sequence A345313. */
seq_t a(idx_t n)
{
    /* Get the prime factorisation of n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t bp  = n / p;                 /* n/p  */
        seq_t   exp;                           /* exponent = lcm(p, n/p) */

        /* Compute lcm(p, n/p): the gcd is either 1 or p. */
        if (bp % p == 0) {                    /* p^2 | n  →  lcm = n/p */
            exp = (seq_t)bp;
        } else {                              /* otherwise lcm = p * (n/p) */
            exp = (seq_t)p * (seq_t)bp;
        }

        /* term = (n/p)^exp */
        seq_t term = big_pow((seq_t)bp, exp);
        sum += term;
    }

    return sum;
}
