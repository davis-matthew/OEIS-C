/*
 * A284289  Number of partitions of n into prime power divisors of n (not
 *          including 1).
 * https://oeis.org/A284289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into its prime‑power divisors
 *            (excluding 1)
 * Range:     u8 n <= 59, u16 n <= 215, u32 n <= 863, u64 n <= 8639,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   The implementation factorises n to obtain all prime power divisors > 1.
   A coin‑change DP counts ordered partitions using those divisors.
   Results are cached so repeated calls are fast.

   Helper functions used:
   - build_primes_upto from "../utils/build_primes_upto.h"
   - floor_sqrt from "../utils/floor_sqrt.h"
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;       /* cached values of a(n) */
static bool *computed = NULL;     /* flags indicating cached entries */
static size_t cache_cap = 0;      /* current capacity of cache */

static uint64_t *prime_list = NULL;   /* global list of primes */
static size_t prime_cnt = 0;          /* number of primes in the list */

/* Ensure that we have a list of primes up to a fixed upper bound */
static void ensure_primes(void)
{
    if (prime_list == NULL) {
        /* 100000 is more than enough for all n up to 1e4 (and beyond) */
        build_primes_upto(100000, &prime_list, &prime_cnt);
    }
}

/* Factorise n using the prime list.  Writes up to 32 prime factors. */
static void factor_small(uint64_t n, uint64_t *factors, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    ensure_primes();

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i) {
        if (m % prime_list[i] == 0) {
            factors[*cnt] = prime_list[i];
            exps[*cnt]     = 0;
            while (m % prime_list[i] == 0) {
                m /= prime_list[i];
                exps[*cnt] += 1;
            }
            (*cnt) += 1;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        factors[*cnt] = m;
        exps[*cnt]     = 1;
        (*cnt) += 1;
    }
}

/* Ensure cache has room for index n */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    bool  *new_comp  = (bool  *)realloc(computed, new_cap * sizeof(bool));
    if (!new_cache || !new_comp)
        abort();

    cache    = new_cache;
    computed = new_comp;

    /* initialise new entries */
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    memset(computed + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));

    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special cases handled by the cache logic below */
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    ensure_cache_size(n);
    if (computed[n])
        return cache[n];

    if (n == 1) {                /* a(1) = 0 */
        computed[n] = true;
        cache[n] = (seq_t)0;
        return cache[n];
    }

    /* Factor n to obtain its prime powers */
    uint64_t factors[32];
    uint64_t exps[32];
    size_t   fac_cnt = 0;
    factor_small((uint64_t)n, factors, exps, &fac_cnt);

    /* Build list of all prime‑power divisors > 1 */
    size_t total_parts = 0;
    for (size_t i = 0; i < fac_cnt; ++i)
        total_parts += exps[i];

    uint64_t *parts = (uint64_t *)malloc(total_parts * sizeof(uint64_t));
    if (!parts)
        abort();

    size_t pidx = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t base = factors[i];
        uint64_t val  = 1;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            val *= base;
            parts[pidx++] = val;
        }
    }

    /* Coin‑change DP to count partitions */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) {
        free(parts);
        abort();
    }
    dp[0] = (seq_t)1;

    for (size_t i = 0; i < total_parts; ++i) {
        idx_t d = (idx_t)parts[i];
        for (idx_t s = d; s <= n; ++s)
            dp[s] += dp[s - d];
    }

    seq_t result = dp[n];

    cache[n] = result;
    computed[n] = true;

    free(dp);
    free(parts);
    return result;
}
