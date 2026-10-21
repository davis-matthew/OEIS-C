/*
 * A259942  Numbers that are larger than or equal to the sum of the cubes of
 *          their prime factors (with multiplicity).
 * https://oeis.org/A259942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 4112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all positive integers m such that
   m >= sum of the cubes of its prime factors (with multiplicity).
   It stores the results in a static array and returns the nth term
   (a(1) is the first term). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function definition required by the build system */
seq_t a(idx_t n)
{
    /* static cache of the first 'cache_len' terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    /* Compute cache on first call */
    if (cache_len == 0) {
        /* Estimate an upper bound that surely contains ≥10000 terms.
           Empirically the density is about 5%, so 300 000 covers the
           requirement comfortably. */
        const idx_t LIMIT = 300000;
        idx_t i, m;
        /* linear sieve for smallest prime factor */
        uint32_t *spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) exit(EXIT_FAILURE);
        uint32_t *primes = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
        if (!primes) exit(EXIT_FAILURE);
        size_t prime_cnt = 0;
        for (i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes[prime_cnt++] = i;
            }
            for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i] && i * (idx_t)primes[j] <= LIMIT; ++j) {
                spf[i * (idx_t)primes[j]] = primes[j];
            }
        }
        free(primes);

        /* Reserve capacity for cache */
        cache_cap = 20000;   /* initial guess */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        /* Enumerate numbers */
        cache_len = 0;
        for (m = 1; m <= LIMIT; ++m) {
            seq_t sum = 0;
            idx_t temp = m;
            if (temp == 1) {
                /* sum stays 0 */
            } else {
                while (temp > 1) {
                    uint32_t p = spf[temp];
                    seq_t pcub = (seq_t)p * p * p;
                    sum += pcub;
                    temp /= p;
                }
            }
            if (sum <= m) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    seq_t *newptr = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!newptr) exit(EXIT_FAILURE);
                    cache = newptr;
                }
                cache[cache_len++] = m;
            }
        }
        free(spf);
    }

    /* Return the requested term (n is 1‑based) */
    if (n == 0 || n > cache_len)
        return 0;            /* undefined for requested n > cache_len */
    return cache[n - 1];
}
