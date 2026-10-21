/*
 * A260160  a(n) = a(n-2) + a(n-6) - a(n-8) with n>8, the first eight terms
 *          are 0 except that for a(5) = a(7) = 1.
 * https://oeis.org/A260160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-6) - a(n-8), a(1..8) = 0,0,0,0,1,0,1,0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t base_init[8] = {0, 0, 0, 0, 1, 0, 1, 0};

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored (initially 8) */

/* Ensure cache holds at least n elements (indices 0..n-1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;
    size_t new_cap = (size_t)n;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base_init[i];
        cache_size = 8;
    } else {
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL) abort();
        cache = new_ptr;
    }
    /* Compute new terms */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
        /* recurrence: a(n) = a(n-2) + a(n-6) - a(n-8) */
        seq_t val = cache[cur - 3] + cache[cur - 7] - cache[cur - 9];
        cache[cur - 1] = val;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n <= 8)
        return base_init[n - 1];
    ensure_cache(n);
    return cache[n - 1];
}
