/*
 * A351368  a(n) = Sum_{p|n, p prime} prime(p).
 * https://oeis.org/A351368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} prime(p) where prime(p) is the p‑th
 *            prime
 * Range:     u8 n <= 58, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Build a small prime sieve once, with enough primes for all n up to ~10000 */
    static bool initialized = false;
    static idx_t *primes = NULL;   /* array of primes, 1‑based access via primes[p-1] */
    static size_t prime_cnt = 0;
    /* 104729 is the 10000‑th prime; we sieve up to 120000 to be safe. */
    const idx_t LIMIT = (idx_t)120000;

    if (!initialized) {
        /* Allocate and fill the sieve */
        bool *isP = (bool *)malloc((LIMIT + 1) * sizeof(bool));
        if (!isP) return 0;          /* allocation failure – unlikely */

        for (idx_t i = 0; i <= LIMIT; ++i) isP[i] = true;
        isP[0] = isP[1] = false;

        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (isP[i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    isP[j] = false;
            }
        }

        /* Count primes and allocate the primes array */
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (isP[i]) prime_cnt++;

        primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
        if (!primes) { free(isP); return 0; }

        /* Store primes in ascending order */
        size_t pos = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (isP[i]) primes[pos++] = i;

        free(isP);
        initialized = true;
    }

    if (n == 1) return 0;

    idx_t t = n;
    seq_t sum = 0;

    /* Factorize n by trial division using the precomputed primes */
    for (size_t idx = 0; idx < prime_cnt && (idx_t)primes[idx] * (idx_t)primes[idx] <= t; ++idx) {
        idx_t p = primes[idx];
        if (t % p == 0) {
            /* p is a prime divisor; add the p‑th prime to the sum */
            sum += (seq_t)primes[p - 1];
            while (t % p == 0) t /= p;
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (t > 1) {
        sum += (seq_t)primes[t - 1];
    }

    return sum;
}
