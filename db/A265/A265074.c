/*
 * A265074  Coordination sequence for (3,3,7) tiling of hyperbolic plane.
 * https://oeis.org/A265074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-5)+a(n-7)-a(n-8) Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=10, a(4)=16, a(5)=26, a(6)=42,
 *            a(7)=67, a(8)=106
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 46, u64 n <= 94, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_size = 0;    /* number of terms stored (indices 0..cache_size-1) */
static size_t cache_cap  = 0;    /* current allocated capacity */

static void ensure_cap(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < n + 1) new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

extern seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* initialize cache with the first 9 terms if not done already */
    if (!cache)
    {
        static seq_t init_vals[9] = { (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)10,
                                      (seq_t)16, (seq_t)26, (seq_t)42,
                                      (seq_t)67, (seq_t)106 };
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        cache_size = 9;
    }

    if (idx < cache_size) return cache[idx];

    ensure_cap(idx);
    for (size_t i = cache_size; i <= idx; ++i)
    {
        /* recurrence valid for i >= 9.  For i <= 8, values are already set. */
        seq_t val = cache[i-1] + cache[i-3] + cache[i-5] + cache[i-7];
        val -= cache[i-8];
        cache[i] = val;
    }
    cache_size = idx + 1;
    return cache[idx];
}
