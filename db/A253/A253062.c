/*
 * A253062  Largest order of a rooted tree that does not contain a rooted
 *          caterpillar subtree of order n.
 * https://oeis.org/A253062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+3*a(n-3)-3*a(n-4) for n>10 Initial values (1‑based
 *            indexing): a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=7,
 *            a(7)=11, a(8)=16, a(9)=23, a(10)=34
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[11] = {
    0,    /* dummy index 0 */
    0,    /* a(1) */
    1,    /* a(2) */
    2,    /* a(3) */
    3,    /* a(4) */
    5,    /* a(5) */
    7,    /* a(6) */
    11,   /* a(7) */
    16,   /* a(8) */
    23,   /* a(9) */
    34    /* a(10) */
};

/* Cache for computed values: cache[1..cache_len] are defined. */
static seq_t *cache      = NULL;      /* NULL until initialized */
static idx_t  cache_len  = 0;          /* highest index computed so far */
static size_t cache_cap  = 0;          /* allocated length of cache */

/* Ensure the cache is allocated up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;    /* need indices 0..n */
    if (new_cap <= cache_cap) return;  /* already large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, guard */

    /* Quick return for the first ten known terms. */
    if (n <= 10) return init[n];

    /* Lazily initialize cache with the first ten terms if needed. */
    if (!cache)
    {
        /* Allocate enough to hold the first ten terms. */
        ensure_cache(10);
        for (idx_t i = 1; i <= 10; ++i) cache[i] = init[i];
        cache_len = 10;
    }

    /* Compute any missing terms up to n. */
    if (n > cache_len)
    {
        /* Allocate storage for the requested index if not already. */
        if ((size_t)n + 1 > cache_cap) ensure_cache(n);

        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* a(i) = a(i-1) + 3*a(i-3) - 3*a(i-4) */
            seq_t term = cache[i-1];
            term += ((seq_t)3) * cache[i-3];
            term -= ((seq_t)3) * cache[i-4];
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
