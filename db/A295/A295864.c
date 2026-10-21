/*
 * A295864  a(n) = hypergeom([-n, -n], [1], 1) * n! / (floor(n/2)!)^2.
 * https://oeis.org/A295864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)!/(n!*floor(n/2)!^2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;          /* floor(n/2) */
    idx_t limit = n * 2;       /* 2*n */

    /* simple sieve up to 2*n to obtain primes */
    size_t sz = (size_t)(limit + 1);
    uint8_t *sieve = (uint8_t *)calloc(sz, 1);
    if (!sieve)
        return (seq_t)0;       /* unlikely */

    sieve[0] = 1;
    if (limit >= 1)
        sieve[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    seq_t res = 1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (sieve[p])
            continue;           /* not prime */

        uint64_t ep = 0;
        idx_t t = p;

        /* exponent of p in (2n)! */
        while (t <= limit) {
            ep += limit / t;
            t *= p;
        }

        /* subtract exponent of p in n! */
        t = p;
        while (t <= n) {
            ep -= n / t;
            t *= p;
        }

        /* subtract exponent of p in (floor(n/2)!)^2 */
        t = p;
        while (t <= m) {
            ep -= 2 * (m / t);
            t *= p;
        }

        if (ep == 0)
            continue;

        res *= seq_pow((seq_t)p, (idx_t)ep);
    }

    free(sieve);
    return res;
}
