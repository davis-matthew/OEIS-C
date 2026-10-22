/*
 * A316628  a(1)=1, a(2)=2, a(3)=2, a(4)=3; a(n) =
 *          a(n-a(n-1))+a(n-1-a(n-2)-a(n-2-a(n-2))) for n > 4.
 * https://oeis.org/A316628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 662, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A316628
 * a(1)=1, a(2)=2, a(3)=2, a(4)=3;
 * a(n) = a(n-a(n-1)) + a(n-1-a(n-2)-a(n-2-a(n-2)))   for n>4
 */
#include <stdlib.h>    /* malloc, realloc, exit, EXIT_FAILURE */

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static size_t cap   = 0;      /* current capacity of the cache array   */
static idx_t  max_n = 0;      /* largest n already computed */

/* Ensure that a(n) is computed and stored in cache[n] */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && n <= max_n)
        return;

    /* Allocate or grow the cache to hold indices up to n */
    if (cache == NULL) {
        cap = 16;
        if ((size_t)n + 1 > cap)
            cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)          /* abort if allocation fails */
            exit(EXIT_FAILURE);
        cache[0] = 0;               /* unused entry for safety */
        /* initialise known terms */
        if (n >= 1) cache[1] = 1;
        if (n >= 2) cache[2] = 2;
        if (n >= 3) cache[3] = 2;
        if (n >= 4) cache[4] = 3;
        max_n = (n < 4) ? n : 4;
    } else if (n > max_n) {
        if ((size_t)n + 1 > cap) {
            size_t new_cap = cap * 2;
            while ((size_t)n + 1 > new_cap)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                exit(EXIT_FAILURE);
            cache = tmp;
            cap = new_cap;
        }

        /* Compute missing terms from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t ai1 = cache[i - 1];
            idx_t  k1 = i - (idx_t)ai1;                /* n-a(n-1) */

            seq_t ai2 = cache[i - 2];
            idx_t  k2a = (i - 2) - (idx_t)ai2;         /* n-2-a(n-2) */
            seq_t ai2a = cache[k2a];
            idx_t  k2 = (i - 1) - (idx_t)ai2 - (idx_t)ai2a; /* n-1-a(n-2)-a(n-2-a(n-2)) */

            cache[i] = cache[k1] + cache[k2];
        }
        max_n = n;
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n<=0, return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[n];
}
