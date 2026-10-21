/*
 * A283267  Smallest b-a such that a < prime(n) < b, where a,b are semiprimes.
 * https://oeis.org/A283267
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A102414(n) - A102415(n) (difference between nearest
 *            semiprimes surrounding the nth prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Return the smallest difference between two neighbouring
   semiprimes surrounding the n‑th prime.
   Sequence offset is 3: a(3) is the first listed term. */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset, value irrelevant */

    /* Static data for memoisation */
    static bool initialized = false;
    static idx_t max_n = 0;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint64_t *spf   = NULL;   /* smallest prime factor */
    static size_t spf_len  = 0;
    static bool   *is_semi = NULL;   /* semiprime flags */
    static size_t limit   = 0;        /* current sieve limit */

    /* (re)compute sieve data if we need a larger n */
    if (!initialized || n > max_n) {
        /* estimate an upper bound for the n‑th prime and a neighbour
           semiprime.  For n <= 10000 a factor 20 plus a buffer is ample. */
        size_t new_limit = (size_t)n * 20 + 5000;
        if (new_limit < 100) new_limit = 100;
        if (new_limit <= limit) new_limit = limit + 5000;  /* always grow */

        /* free old tables if they exist */
        free(primes);
        free(spf);
        free(is_semi);

        limit   = new_limit;
        spf_len = limit + 1;
        spf     = (uint64_t*)malloc(sizeof(uint64_t) * spf_len);
        memset(spf, 0, sizeof(uint64_t) * spf_len);

        size_t primes_cap = limit / 2 + 10;
        primes = (uint64_t*)malloc(sizeof(uint64_t) * primes_cap);
        prime_cnt = 0;

        /* Sieve of Eratosthenes together with smallest prime factor */
        for (uint64_t i = 2; i <= limit; ++i) {
            if (!spf[i]) {                 /* i is prime */
                spf[i] = i;
                if (prime_cnt < primes_cap) primes[prime_cnt++] = i;
                if (i <= limit / i) {
                    for (uint64_t j = i * i; j <= limit; j += i)
                        if (!spf[j]) spf[j] = i;
                }
            }
        }

        /* Build semiprime flag array */
        is_semi = (bool*)malloc(sizeof(bool) * spf_len);
        memset(is_semi, 0, sizeof(bool) * spf_len);
        for (uint64_t i = 4; i <= limit; ++i) {
            uint64_t p = spf[i];
            uint64_t q = i / p;
            if (q > 1 && spf[q] == q)   /* q is prime -> semiprime */
                is_semi[i] = true;
        }

        initialized = true;
        max_n = n;
    }

    /* If the current prime table is still too small, enlarge it */
    if ((idx_t)prime_cnt < n) {
        size_t new_limit = limit * 2 + 10000;
        free(primes);
        free(spf);
        free(is_semi);

        limit   = new_limit;
        spf_len = limit + 1;
        spf     = (uint64_t*)malloc(sizeof(uint64_t) * spf_len);
        memset(spf, 0, sizeof(uint64_t) * spf_len);

        size_t primes_cap = limit / 2 + 10;
        primes = (uint64_t*)malloc(sizeof(uint64_t) * primes_cap);
        prime_cnt = 0;

        for (uint64_t i = 2; i <= limit; ++i) {
            if (!spf[i]) {
                spf[i] = i;
                if (prime_cnt < primes_cap) primes[prime_cnt++] = i;
                if (i <= limit / i) {
                    for (uint64_t j = i * i; j <= limit; j += i)
                        if (!spf[j]) spf[j] = i;
                }
            }
        }

        is_semi = (bool*)malloc(sizeof(bool) * spf_len);
        memset(is_semi, 0, sizeof(bool) * spf_len);
        for (uint64_t i = 4; i <= limit; ++i) {
            uint64_t p = spf[i];
            uint64_t q = i / p;
            if (q > 1 && spf[q] == q)
                is_semi[i] = true;
        }

        max_n = n;
    }

    /* The nth prime (1‑based) */
    uint64_t p = primes[n - 1];

    /* Search downward for the nearest semiprime < p */
    uint64_t lower = p - 1;
    while (lower >= 4 && !is_semi[lower]) --lower;

    /* Search upward for the nearest semiprime > p */
    uint64_t upper = p + 1;
    while (upper <= limit && !is_semi[upper]) ++upper;

    /* The difference fits into seq_t for the required n */
    return (seq_t)(upper - lower);
}
