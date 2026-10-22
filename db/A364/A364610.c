/*
 * A364610  Centered pentagonal numbers which are products of three distinct
 *          primes.
 * https://oeis.org/A364610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 7715, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Centered pentagonal numbers that are products of three distinct primes.
   a(n) is the n-th such number (n ≥ 1).  The function searches the centered
   pentagonal numbers (5k²+5k+2)/2 until it finds enough terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

static seq_t *cache   = NULL;   /* cached terms */
static idx_t  cache_len  = 0;   /* number of cached terms */
static idx_t  cache_cap  = 0;   /* allocated capacity */
static idx_t  m_counter  = 1;   /* next centered‑pentagonal index to test */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Test whether x is a product of exactly three distinct primes (each to power 1). */
static bool three_distinct_primes(uint64_t x)
{
    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(x, pr, ex, &cnt);
    if (cnt != 3)
        return false;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] != 1)
            return false;
    return true;
}

/* Return the n-th centered pentagonal number that is a product of three distinct primes. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for 0; return 0 for safety */
        return 0;

    if (n > cache_len) {
        idx_t needed = n;
        ensure_cache_capacity(needed);

        while (cache_len < needed) {
            /* Compute the centered pentagonal number for current k. */
            seq_t k  = (seq_t)m_counter;
            seq_t pk = ((seq_t)5 * k * k + (seq_t)5 * k + 2) / 2;

            /* Test for exactly three distinct prime factors. */
            if (three_distinct_primes((uint64_t)pk)) {
                cache[cache_len++] = pk;
            }

            m_counter++;      /* Advance to the next k regardless of success. */
        }
    }

    return cache[n - 1];
}
