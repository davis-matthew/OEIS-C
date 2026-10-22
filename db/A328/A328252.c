/*
 * A328252  Numbers that are not squarefree, but whose arithmetic derivative
 *          (A003415) is.
 * https://oeis.org/A328252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1793, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328252: numbers that are not squarefree, but whose arithmetic
   derivative is squarefree.  The function a(n) returns the nth term
   (1‑based) for any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers provided by the build system */
#include "../utils/is_squarefree.h"
#include "../utils/factor_u64.h"

/* Compute the arithmetic derivative of a uint64_t number.
   Formula: d(n) = n * Σ (e_p / p) over prime powers p^e dividing n */
static seq_t arithmetic_derivative_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term = (seq_t)(n / primes[i]) * (seq_t)exps[i];
        der += term;
    }
    return der;
}

/* Static cache for previously found terms */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Grow the cache array to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete [] cache;
#else
    new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Initialise cache capacity */
    if (cache_cap == 0) grow_cache(128);

    /* Start search from the integer after the last found term */
    uint64_t cur = 1;
    if (cache_len > 0) cur = (uint64_t)cache[cache_len - 1] + 1;

    while (cache_len < (size_t)n)
    {
        seq_t val = (seq_t)cur;

        /* Check condition: not squarefree, but derivative is squarefree */
        if (!is_squarefree(val))
        {
            seq_t der = arithmetic_derivative_uint64(cur);
            if (is_squarefree(der))
            {
                /* Append to cache */
                if (cache_len == cache_cap) grow_cache(cache_cap * 2);
                cache[cache_len++] = val;
                if (cache_len == (size_t)n) break;
            }
        }
        ++cur;
    }
    return cache[n - 1];
}
