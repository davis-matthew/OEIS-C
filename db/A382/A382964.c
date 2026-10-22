/*
 * A382964  Number of k <= n such that rad(k) divides n and g * k > n where g
 *          is the largest prime factor of n and rad = A007947.
 * https://oeis.org/A382964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≤ n : rad(k) | n ∧ largest_prime_factor(n) * k > n }
 * Range:     u8 n <= 1089, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global caches for rad(k) and largest prime factor of k */
static uint64_t *rad_cache = NULL;      /* rad(k) for k ≤ cache_size */
static uint64_t *lpf_cache = NULL;      /* largest prime factor of k ≤ cache_size */
static idx_t cache_size = 0;            /* largest n for which caches are valid */

/* Ensure caches are filled up to 'limit' */
static void ensure_caches(idx_t limit)
{
    if (limit <= cache_size) return;           /* already valid */

    /* Allocate memory for caches (C++ casts added) */
    rad_cache = (uint64_t*)realloc(rad_cache, (limit + 1) * sizeof(uint64_t));
    lpf_cache = (uint64_t*)realloc(lpf_cache, (limit + 1) * sizeof(uint64_t));
    if (!rad_cache || !lpf_cache)
        exit(EXIT_FAILURE);

    /* Smallest prime factor array */
    uint64_t *spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!spf)
        exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                     /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    spf[1] = 1; /* helper value */

    /* Base cases */
    rad_cache[0] = 1; rad_cache[1] = 1;
    lpf_cache[0] = 1; lpf_cache[1] = 1;

    /* Compute rad and lpf for each number */
    for (idx_t i = 2; i <= limit; ++i) {
        uint64_t p   = spf[i];
        idx_t   rest = i / p;

        /* rad(i) */
        rad_cache[i] = rad_cache[rest];
        if (rest % p != 0) rad_cache[i] *= p;

        /* largest prime factor */
        lpf_cache[i] = lpf_cache[rest];
        if (p > lpf_cache[i]) lpf_cache[i] = p;
    }

    cache_size = limit;
    free(spf);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* a(0) not used but defined for safety */
    ensure_caches(n);

    seq_t count = 0;                            /* count of qualifying k */
    seq_t g     = (seq_t)lpf_cache[n];           /* largest prime factor of n */

    for (idx_t k = 1; k <= n; ++k) {
        if (n % rad_cache[k] == 0) {
            if (g * (seq_t)k > (seq_t)n)
                ++count;
        }
    }
    return count;
}
