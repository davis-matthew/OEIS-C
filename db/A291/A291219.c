/*
 * A291219  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^3.
 * https://oeis.org/A291219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A291219 – p-INVERT of (0,1,0,1,0,1,...)
 * Recurrence (n ≥ 7):
 *   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 3*a(n-4) + a(n-5) + a(n-6)
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=3, a(3)=5, a(4)=11, a(5)=21, a(6)=42
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* storage for computed terms */
static size_t cache_size = 0; /* number of terms currently stored */
static size_t cache_cap  = 0; /* allocated capacity of cache */

/* Ensure that cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_size >= target)
        return; /* already computed */

    /* Expand memory if necessary */
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < target)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (size_t i = cache_size; i < target; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)5;
        else if (i == 4)
            cache[i] = (seq_t)11;
        else if (i == 5)
            cache[i] = (seq_t)21;
        else if (i == 6)
            cache[i] = (seq_t)42;
        else {
            /* recurrence: a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 3*a(n-4) + a(n-5) + a(n-6) */
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            seq_t v5 = cache[i-5];
            seq_t v6 = cache[i-6];
            cache[i] = v1 + (v2 * (seq_t)3) - v3 - (v4 * (seq_t)3) + v5 + v6;
        }
    }
    cache_size = target;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
