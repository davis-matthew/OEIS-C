/*
 * A257442  Number of (n+2) X (3+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257442
 *
 * Empirical recurrence for n > 6:
 *   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4)
 * Initial values:
 *   a(1)=39, a(2)=41, a(3)=47, a(4)=59, a(5)=77, a(6)=104
 *
 * The function memoises computed values so that any call order works
 * and each term is calculated only once.  It uses only C11 and
 * C++‑17 compatible constructs, and works for all supported integer
 * sizes (uint8_t … unsigned __int128, or arbitrary‑precision).
 */

#include <stdlib.h>

/* Try to allocate an array of n seq_t elements; return NULL if n==0. */
static seq_t *alloc_array(idx_t n)
{
    return n ? (seq_t *)malloc(n * sizeof(seq_t)) : NULL;
}

/* Free an array previously allocated with alloc_array. */
static void free_array(seq_t *p)
{
    free(p);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe to return 0 */

    /* Static cache of computed values */
    static seq_t *cache        = NULL;
    static idx_t  cache_size   = 0;

    /* Initialise cache with the first six terms on first call */
    if (!cache) {
        cache = alloc_array(6);
        if (!cache) return 0;      /* allocation failure: give up */
        cache[0] = 39;   /* a(1) */
        cache[1] = 41;   /* a(2) */
        cache[2] = 47;   /* a(3) */
        cache[3] = 59;   /* a(4) */
        cache[4] = 77;   /* a(5) */
        cache[5] = 104;  /* a(6) */
        cache_size = 6;
    }

    /* Return already computed value if possible */
    if (n <= cache_size) return cache[n-1];

    /* Grow the cache to hold the new terms */
    seq_t *new_cache = alloc_array(n);
    if (!new_cache) return 0;      /* allocation failure */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    free_array(cache);
    cache = new_cache;

    /* Compute remaining terms using the recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i is 0‑based index of a(i+1), so we need a(i), a(i-1), a(i-2), a(i-3) */
        seq_t val = 2 * cache[i-1];   /* 2*a(i) */
        val -= cache[i-2];            /* -a(i-1) */
        val += cache[i-3];            /* +a(i-2) */
        val -= cache[i-4];            /* -a(i-3) */
        cache[i] = val;
    }

    cache_size = n;
    return cache[n-1];
}
