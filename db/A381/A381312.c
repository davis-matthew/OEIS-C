/*
 * A381312  Numbers whose powerful part (A057521) is a power of a prime with
 *          an odd exponent >= 3 (A056824).
 * https://oeis.org/A381312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates A381312 by testing successive integers
 * for the defining property:
 *   An integer n qualifies if its prime factorisation contains exactly
 *   one prime with an odd exponent ≥ 3, and all other primes occur to
 *   exponent 1.  In other words: n = p^(2k+1) * m where p is prime,
 *   k ≥ 1 and m is square‑free and coprime to p.  The function caches
 *   the values so that any call order is supported. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t *, size_t, size_t) */

/* ---------- Helper: check whether x qualifies --------------------- */
static bool qualifies(uint64_t x)
{
    /* max distinct prime factors for a 64‑bit integer is below 64,
       but we allow a few extra slots for safety. */
    static uint64_t primes[128];
    static uint64_t exps[128];
    size_t cnt;

    factor_u64(x, primes, exps, &cnt);

    size_t nonunit = 0;            /* primes with exponent > 1 */
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e == 1)
            continue;               /* unit prime factor, allowed */

        /* Exponent > 1 must be the single non‑unit exponent,
           it must be odd and at least 3. */
        if (++nonunit > 1)
            return false;           /* more than one non‑unit exponent */
        if ((e & 1ULL) == 0 || e < 3)
            return false;           /* even exponent or < 3 */
    }
    return nonunit == 1;            /* exactly one suitable exponent */
}

/* ---------- Cache for computed terms ------------------------------ */
static seq_t *cache = NULL;   /* array of computed a(n) values */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static uint64_t tested_max = 1; /* largest integer examined so far */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < need)
        newcap <<= 1;

    cache = seq_realloc(cache, cache_cap, newcap);
    cache_cap = newcap;
}

/* Generate terms until we have at least 'n' terms in the cache. */
static void grow_cache_to(idx_t n)
{
    if ((idx_t)cache_len >= n)
        return;

    while ((idx_t)cache_len < n)
    {
        ++tested_max;                     /* next candidate integer */
        if (qualifies(tested_max))
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)tested_max;
        }
    }
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0, return 0 as a safe default */

    grow_cache_to(n);
    return cache[n - 1];
}
