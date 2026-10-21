/*
 * A297901  Number of n X 2 0..1 arrays with every element equal to 0, 1, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n > 6): a(n) = a(n‑1) + 3*a(n‑2)
 *            + 8*a( n‑3 ) − 4*a(n‑4) − 16*a(n‑5) Initial values: a(1)=2,
 *            a(2)=4, a(3)=3, a(4)=13, a(5)=34, a(6)=73
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, abort, NULL, size_t */

/* Static cache for memoisation (1‑based index → a(index)) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (number of slots)     */
static idx_t cache_len = 0;    /* highest index computed so far (≤ cache_cap) */

/* Ensure the cache can hold indices up to `n` (1‑based). */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap >= n)
        return;
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && n > 0)  /* abort on allocation failure for n>0 */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first six terms of the sequence. */
static void initialize_cache(void)
{
    ensure_capacity(6);
    cache[0] = (seq_t)2;    /* a(1) */
    cache[1] = (seq_t)4;    /* a(2) */
    cache[2] = (seq_t)3;    /* a(3) */
    cache[3] = (seq_t)13;   /* a(4) */
    cache[4] = (seq_t)34;   /* a(5) */
    cache[5] = (seq_t)73;   /* a(6) */
    cache_len = 6;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* unused formality */
        return (seq_t)0;

    if (cache_len == 0)          /* first call – initialise cache */
        initialize_cache();

    /* If the value is already computed, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Prepare storage for the missing terms. */
    ensure_capacity(n);

    /* Compute terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* a(i) = a(i‑1) + 3*a(i‑2) + 8*a(i‑3) − 4*a(i‑4) − 16*a(i‑5) */
        seq_t val = cache[i - 2]                       /* a(i‑1) */
                  + (seq_t)3 * cache[i - 3]           /* 3*a(i‑2) */
                  + (seq_t)8 * cache[i - 4]           /* 8*a(i‑3) */
                  - (seq_t)4 * cache[i - 5]           /* −4*a(i‑4) */
                  - (seq_t)16 * cache[i - 6];         /* −16*a(i‑5) */
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
