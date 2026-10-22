/*
 * A320857  a(n) = Pi(8,5)(n) + Pi(8,7)(n) - Pi(8,1)(n) - Pi(8,3)(n) where
 *          Pi(a,b)(x) denotes the number of primes in the arithmetic
 *          progression a*k + b less than or equal to x.
 * https://oeis.org/A320857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Pi(8,5)(n) + Pi(8,7)(n) - Pi(8,1)(n) - Pi(8,3)(n) * (sum
 *            over primes ≤ n of -Jacobi(-2,p), i.e. (#p ≡5,7 mod 8) – (#p
 *            ≡1,3 mod 8))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Build a list of all primes ≤ n */
    size_t cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        /* 2 is the only even prime and is ignored (remainder 2) */
        uint8_t rem = (uint8_t)(p & 7);    /* p % 8 */
        if (rem == 5 || rem == 7)
            res += 1;
        else if (rem == 1 || rem == 3)
            res -= 1;
    }

    free(primes);
    return res;
}
