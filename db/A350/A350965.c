/*
 * A350965  a(n) = sqrt(6*A138288(n)^2 - 2).
 * https://oeis.org/A350965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=22, a(n)=10*a(n-1)-a(n-2) for n>=2 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *cache      = NULL;   /* cached values */
static idx_t  cache_len  = 0;       /* number of cached values (valid entries 0..cache_len-1) */
static size_t cache_cap  = 0;       /* capacity of cache array */

static void seq_ensure_cache(idx_t n)
{
    if (cache_len == 0) {                /* initialise cache */
        /* initial capacity must be at least 2 to hold a(0) and a(1) */
        cache_cap = (n + 1) > 2 ? (size_t)(n + 1) : 2;
        cache = alloc_cache(cache_cap);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)22;
        cache_len = 2;
    }
    if (cache_len - 1 < n) {             /* grow and compute up to n */
        idx_t   needed = n + 1;
        size_t  new_cap = cache_cap;
        while (new_cap < (size_t)needed) new_cap *= 2;
        if (new_cap != cache_cap)
            grow_seq_array(&cache, &cache_cap, new_cap);
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)10 - cache[i - 2];
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    seq_ensure_cache(n);
    return cache[n];
}
