/*
 * A390470  Number of positive z <= n such that z^2 - 25^g is relatively prime
 *          to n (for any particular positive integer g).
 * https://oeis.org/A390470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: multiplicative function given by a(2^e)   = 2^(e-1),
 *                        a(5^e)   = 4*5^(e-1),
 *                        a(p^e)   = (p-2)*p^(e-1)  for odd primes p != 5.
 */
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* arrays large enough for the number of distinct prime factors of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* compute p^(e-1) */
        seq_t powpe = big_pow((seq_t)p, (seq_t)(e - 1));
        seq_t mult;
        if (p == 2ULL)
            mult = powpe;                     /* 2^(e-1) */
        else if (p == 5ULL)
            mult = powpe * (seq_t)4;           /* 4*5^(e-1) */
        else
            mult = powpe * (seq_t)(p - 2ULL);  /* (p-2)*p^(e-1) */
        res = res * mult;
    }
    return res;
}
