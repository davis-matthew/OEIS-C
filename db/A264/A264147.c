/*
 * A264147  a(n) = n*F(n+1) - (n+1)*F(n), where F = A000045.
 * https://oeis.org/A264147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*F(n+1) – (n+1)*F(n) * Recurrence (for n≥4): a(n) =
 *            2*a(n-1) + a(n-2) – 2*a(n-3) – a(n-4) * Initial values: a(0)=0,
 *            a(1)=‑1, a(2)=1, a(3)=1
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 39, i64 n <= 84, i128 n <= 174,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 498 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
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

static seq_t *cache      = NULL; /* cached values of a(n) */
static idx_t  cache_len  = 0;    /* number of valid entries      */
static idx_t  cache_cap  = 0;    /* allocated capacity          */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* initialise the cache with the first four terms */
    if (cache_len == 0) {
        cache_cap  = 4;
        cache_len  = 4;
        cache      = alloc_seq_array(cache_cap);
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
    }

    /* return directly if already computed */
    if (n < cache_len) return cache[n];

    /* grow the cache to accommodate indices up to n */
    idx_t new_cap = n + 1;
    if (new_cap > cache_cap) {
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* fill new entries using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-2) – 2*a(i-3) – a(i-4) */
        cache[i] = 2 * cache[i - 1] + cache[i - 2]
             - 2 * cache[i - 3] - cache[i - 4];
    }
    cache_len = new_cap;

    return cache[n];
}
