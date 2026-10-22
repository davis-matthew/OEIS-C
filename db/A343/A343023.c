/*
 * A343023  Number of cyclic cubic fields with discriminant n^2.
 * https://oeis.org/A343023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(ω(n)-1) if n>1 and n is built only from primes ≡1 (mod
 *            3) (each with exponent 1) and possibly a single factor 3²;
 *            otherwise a(n)=0. Here ω(n) is the number of distinct prime
 *            factors of n (counting 3 if it appears as 3²). For n=1, a(1)=0.
 *            This implements the definition of OEIS A343023
 * Range:     u8 n <= 9661, u16 n <= 9661, u32 n <= 9661, u64 n <= 9661,
 *            u128 n <= 9661, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    uint64_t exp3 = 0;

    /* Count powers of 3 */
    while (m % 3 == 0) {
        exp3++;
        m /= 3;
        if (exp3 > 2)   /* 3^2 allowed only once */
            return (seq_t)0;
    }
    if (exp3 == 1)       /* isolated 3 is forbidden */
        return (seq_t)0;

    size_t omega = 0;    /* distinct prime factors of n, counting 3 if exp3==2 */

    if (exp3 == 2)
        omega++;         /* count the prime 3 */

    /* Factor the remaining part of n */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp != 1)                    /* exponents >1 are forbidden */
                return (seq_t)0;
            if (p % 3 != 1)                  /* only p ≡1 (mod 3) allowed */
                return (seq_t)0;
            omega++;                         /* count this distinct prime */
        }
    }

    if (m > 1) { /* remaining prime factor after division by all p² */
        if (m % 3 != 1)          /* must be ≡1 (mod 3) */
            return (seq_t)0;
        omega++;                 /* count this prime */
    }

    /* omega is now the number of distinct prime factors of n */
    /* For valid n, omega ≥ 1 (since n>1). */
    return pow2((idx_t)(omega - 1));
}
