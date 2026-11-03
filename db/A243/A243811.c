/*
 * A243811  Numbers k such that 2*k+3 and 2*k+5 are both prime.
 * https://oeis.org/A243811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = the n-th integer k such that 2*k+3 and 2*k+5 are prime
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 990 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;          /* stored sequence values (k) */
static idx_t cache_len = 0;          /* number of values cached */
static idx_t cache_cap = 0;          /* current capacity of cache */
static idx_t k_next = 0;             /* next k to test */

/* Resize the cache array if needed */
static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Find the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; undefined for 0 */
        return (seq_t)0;

    /* Expand cache until it contains at least n values */
    while (cache_len < n)
    {
        /* Test candidate k = k_next */
        idx_t k = k_next;
        idx_t p = 2 * k + 3;
        idx_t q = 2 * k + 5;

        if (is_prime(p) && is_prime(q))
        {
            /* Append k to cache */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = (seq_t)k;
            cache_len++;
            k_next = k + 1;
        }
        else
        {
            k_next = k + 1;
        }
    }

    return cache[n - 1];
}
