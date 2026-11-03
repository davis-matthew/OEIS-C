/*
 * A258622  a(1) = 2, a(n+1) = a(n) * (1 + n + a(n))/n.
 * https://oeis.org/A258622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(n+1) = a(n)*(1+n+a(n))/n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cache_max = 0;
    static size_t  cache_cap = 0;

    if (n == 0)                /* undefined, but harmless */
        return 0;

    /* Initial cache setup on first call */
    if (!cache) {
        size_t init_cap = (size_t)(n > 1 ? n + 1 : 2);
        cache = seq_alloc_new_array(init_cap);
        cache_cap = init_cap;
        cache[1] = (seq_t)2;
        cache_max = 1;
        if (n == 1)
            return cache[1];
    }

    /* Ensure the array can hold the requested index */
    if ((size_t)(n + 1) > cache_cap)
        grow_seq_array(&cache, &cache_cap, (size_t)(n + 1));

    /* Fill in any missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t im1 = i - 1;
        seq_t num = prev * ((seq_t)im1 + (seq_t)1 + prev);
        cache[i] = num / ((seq_t)im1);
    }
    cache_max = n;
    return cache[n];
}
