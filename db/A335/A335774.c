/*
 * A335774  Numbers k such that in prime factorization of k the second
 *          smallest factor is 7.
 * https://oeis.org/A335774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-14) - a(n-15) Initial values: a(1)=14,
 *            a(2)=21, a(3)=28, a(4)=35, a(5)=56, a(6)=63, a(7)=98, a(8)=112,
 *            a(9)=147, a(10)=154, a(11)=175, a(12)=182, a(13)=189, a(14)=196,
 *            a(15)=224 (a(0) is undefined, return 0)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Precomputed initial 15 terms */
static const seq_t init_vals[16] = {
    0,     /* index 0 unused */
    14,    /* a(1) */
    21,    /* a(2) */
    28,    /* a(3) */
    35,    /* a(4) */
    56,    /* a(5) */
    63,    /* a(6) */
    98,    /* a(7) */
    112,   /* a(8) */
    147,   /* a(9) */
    154,   /* a(10) */
    175,   /* a(11) */
    182,   /* a(12) */
    189,   /* a(13) */
    196,   /* a(14) */
    224    /* a(15) */
};

/* 1‑indexed cache of computed values */
static seq_t *cache = NULL;    /* pointer to allocated array */
static idx_t cache_max = 0;    /* largest n for which cache[n] is valid */
static size_t cache_size = 0;  /* size of allocated array (>= cache_max+1) */

/* Ensure cache contains values up to index n. */
static void grow_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Determine new allocated size (at least n+1, exponential growth). */
    size_t new_size = (cache_size == 0 ? 16 : cache_size);
    while (new_size <= (size_t)n)
        new_size <<= 1;

    /* Reallocate memory; cast from void* to seq_t*. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory allocation failure */

    cache = new_cache;
    cache_size = new_size;

    /* Compute the newly required entries. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 15)
            cache[i] = init_vals[i];
        else
            cache[i] = cache[i - 1] + cache[i - 14] - cache[i - 15];
    }
    cache_max = n;
}

/* Public sequence function.  Index n ≥ 1; for n < 1 it returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    grow_cache(n);
    return cache[n];
}
