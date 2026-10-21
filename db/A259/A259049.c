/*
 * A259049  Number of self-complementary plane partitions in a (2n)-cube.
 * https://oeis.org/A259049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=0..n-1} i!^2 (i+2n)!^2 / (i+n)!^4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset */
    if (n == 0)
        return 1;  /* a(0) = 1 */

    /* upper bound for prime sieve: 3n */
    idx_t limit = 3 * n;
    size_t L = (size_t)limit;       /* for array indexing */

    /* sieve for primes up to L */
    char *isprime = (char*)malloc((L + 1) * sizeof(char));
    if (!isprime)
        return 1;  /* fallback */

    for (size_t i = 0; i <= L; ++i)
        isprime[i] = 1;
    if (L >= 0) isprime[0] = 0;
    if (L >= 1) isprime[1] = 0;

    for (size_t i = 2; i * i <= L; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= L; j += i)
                isprime[j] = 0;
        }
    }

    /* collect primes into array */
    size_t maxp = L / 2 + 1;
    size_t *primes = (size_t*)malloc(maxp * sizeof(size_t));
    if (!primes) {
        free(isprime);
        return 1;
    }
    size_t pcount = 0;
    for (size_t i = 2; i <= L; ++i)
        if (isprime[i])
            primes[pcount++] = i;

    /* compute the product over primes */
    seq_t result = 1;

    for (size_t pi = 0; pi < pcount; ++pi) {
        size_t p = primes[pi];
        idx_t e = 0;
        idx_t S1 = 0, S2 = 0, S3 = 0;

        for (idx_t m = 1; m <= limit; ++m) {
            /* exponent of p in m */
            idx_t cnt = 0;
            idx_t tmp = m;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            e += cnt;

            /* accumulate sums for the three ranges */
            if (m <= n - 1)            S1 += e;
            else if (m >= n && m <= 2 * n - 1)  S3 += e;
            else if (m >= 2 * n && m <= 3 * n - 1) S2 += e;
        }

        /* exponent for this prime in the final product */
        idx_t exp = 2 * S1 + 2 * S2 - 4 * S3;

        if (exp > 0) {
            seq_t powv = big_pow((seq_t)p, (seq_t)exp);
            result = result * powv;
        }
    }

    free(isprime);
    free(primes);
    return result;
}
