/*
 * A251223  Number of (n+1)X(3+1) 0..1 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 39*a(n-2) - 48*a(n-3) - 210*a(n-4) * +
 *            88*a(n-5) + 260*a(n-6) - 199*a(n-7) + 36*a(n-8) * Initial terms:
 *            a(1)=171, a(2)=1892, a(3)=20782, a(4)=228689, * a(5)=2515011,
 *            a(6)=27662994, a(7)=304255924, a(8)=3346446223
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m) */
#include "../utils/seq_realloc.h" /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* Cache for computed terms */
static seq_t *cache      = NULL; /* a(1)..a(k) stored at cache[0]..cache[k-1] */
static size_t cache_cap = 0;     /* allocated slots in cache */
static size_t cache_sz  = 0;     /* number of terms currently cached */

/* Ensure cache has capacity for at least ‘needed’ elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Initialise the first 8 terms of the sequence in the cache */
static void init_cache(void)
{
    if (cache_sz >= 8)
        return;
    ensure_cache_capacity(8);
    cache[0] = (seq_t)171;
    cache[1] = (seq_t)1892;
    cache[2] = (seq_t)20782;
    cache[3] = (seq_t)228689;
    cache[4] = (seq_t)2515011;
    cache[5] = (seq_t)27662994;
    cache[6] = (seq_t)304255924;
    cache[7] = (seq_t)3346446223;
    cache_sz = 8;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* out of bounds */

    /* Convert idx_t to size_t safely (the sequence is only asked for n up to ~1e4,
       so the cast is safe on any build that supports 64‑bit n). */
    size_t idx = (size_t)n;

    init_cache();

    /* If we have already computed up to idx, just return it */
    if (idx <= cache_sz)
        return cache[idx - 1];

    /* Compute missing terms up to idx */
    for (size_t i = cache_sz + 1; i <= idx; ++i)
    {
        /* Recurrence uses a(i-1)..a(i-8) */
        seq_t t1 = cache[i - 2];   /* a(i-1) */
        seq_t t2 = cache[i - 3];   /* a(i-2) */
        seq_t t3 = cache[i - 4];   /* a(i-3) */
        seq_t t4 = cache[i - 5];   /* a(i-4) */
        seq_t t5 = cache[i - 6];   /* a(i-5) */
        seq_t t6 = cache[i - 7];   /* a(i-6) */
        seq_t t7 = cache[i - 8];   /* a(i-7) */
        seq_t t8 = cache[i - 9];   /* a(i-8) */

        seq_t pos = mul_small(t1, 8);
        pos += mul_small(t2, 39);
        pos += mul_small(t5, 88);
        pos += mul_small(t6, 260);
        pos += mul_small(t8, 36);

        seq_t neg = mul_small(t3, 48);
        neg += mul_small(t4, 210);
        neg += mul_small(t7, 199);

        seq_t val = pos - neg; /* guaranteed non‑negative */

        ensure_cache_capacity(i);
        cache[i - 1] = val;
    }

    cache_sz = idx;
    return cache[idx - 1];
}
