/*
 * A334344  Binary Moran numbers: numbers k such that k divided by its binary
 *          weight (A000120) is a prime number.
 * https://oeis.org/A334344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k is the n‑th integer such that k divided by its
 *            binary weight (popcount(k)) is a prime number
 * Range:     u8 n <= 17, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9728 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Efficient popcount for seq_t by repeated x &= x-1. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        x &= (x - 1);
        ++c;
    }
    return c;
}

/* Cached sequence values and state variables. */
static seq_t *cache      = NULL;  /* array of found terms   */
static size_t cache_size = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity     */
static seq_t last_checked = 0;    /* last number examined   */

/* Return the n-th term of OEIS A334344 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but guard against bad calls */

    /* Return cached value if already known. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Ensure enough capacity for new elements. */
    if (cache_size == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache) abort();
        if (cache)
            mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
        mem_free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Continue searching until we have n terms. */
    while (cache_size < (size_t)n)
    {
        /* Move to next candidate integer. */
        last_checked = last_checked + (seq_t)1;

        /* Compute binary weight (popcount). */
        unsigned w = popcount_seq_t(last_checked);
        if (w == 0)
            continue; /* safety, should never happen for positive numbers */

        /* Check divisibility and primality of quotient. */
        if ((last_checked % (seq_t)w) == (seq_t)0)
        {
            seq_t q = last_checked / (seq_t)w;
            if (isPrimeGeneric(q))
            {
                cache[cache_size++] = last_checked;
                /* Ensure capacity for next insertion. */
                if (cache_size == cache_cap)
                {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_cache = mem_new(seq_t, new_cap);
                    if (!new_cache) abort();
                    mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
                    mem_free(cache);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
            }
        }
    }

    return cache[(size_t)n - 1];
}
