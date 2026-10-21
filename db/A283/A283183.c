/*
 * A283183  Number of partitions of n into a prime and a square of an
 *          arbitrary integer.
 * https://oeis.org/A283183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) ? 1 : 0) + 2 * Σ_{m=1}^{⌊√(n‑2)⌋} [prime(n‑m²)]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fsqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A283183 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/fsqrt_u64.h"

/* Static prime sieve storage */
static bool *prime_sieve = NULL;     /* prime_sieve[i] == true iff i is prime */
static idx_t sieve_limit = 0;        /* current sieve size */

/* Rebuild the sieve up to 'new_limit' (inclusive) */
static void rebuild_sieve(idx_t new_limit)
{
    free(prime_sieve);
    prime_sieve = (bool*)calloc((size_t)(new_limit + 1), sizeof(bool));
    if (!prime_sieve) return;   /* allocation failed; queries thereafter will fail */

    if (new_limit >= 0) prime_sieve[0] = false;
    if (new_limit >= 1) prime_sieve[1] = false;

    for (idx_t i = 2; i <= new_limit; ++i) prime_sieve[i] = true;

    idx_t sqrt_lim = fsqrt_u64(new_limit);
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (prime_sieve[p]) {
            /* start from p*p, avoid overflow by using 128 bits */
            __uint128_t start = (__uint128_t)p * p;
            for (idx_t mult = (idx_t)start; mult <= new_limit; mult += p)
                prime_sieve[mult] = false;
        }
    }
    sieve_limit = new_limit;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* offset: a(1) = 0 */

    if (n > sieve_limit)                 /* extend sieve if necessary */
        rebuild_sieve(n);

    seq_t count = 0;

    if (prime_sieve[n]) count += 1;       /* m = 0 representation */

    if (n >= 3) {
        idx_t m_max = fsqrt_u64(n - 2);
        for (idx_t m = 1; m <= m_max; ++m) {
            idx_t p = n - m * m;
            if (p >= 2 && prime_sieve[p]) count += 2;   /* ±m */
        }
    }

    return count;
}
