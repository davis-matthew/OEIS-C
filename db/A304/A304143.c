/*
 * A304143  Number of nX2 0..1 arrays with every element unequal to 1, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) (n >
 *            6) Initial values: a(1)=1, a(2)=7, a(3)=14, a(4)=33, a(5)=70,
 *            a(6)=157 Offset: 1 Sequence A304143: Number of n×2 0..1 arrays
 *            with no king‑move adjacency equal to 1, 2, 3 or 6, with upper
 *            left element zero
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, abort */

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused   */
static idx_t cache_size = 0;     /* size of allocated array           */
static idx_t computed_upto = 0;  /* highest index with a valid value */

/* Ensure the cache is allocated at least up to index 'upto' (inclusive). */
static void ensure_cache(idx_t upto)
{
    if (cache && upto < cache_size)
        return;                       /* already large enough */

    size_t newsize = (size_t)(upto + 1);   /* indices 0..upto           */
    seq_t *newptr = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newptr)
        abort();                     /* allocation failure          */
    cache = newptr;
    cache_size = newsize;
}

/* Initialise cache with the known initial terms. */
static void init_cache(void)
{
    if (cache)
        return;                       /* already initialised        */

    /* Allocate space for indices 0..6 (0 unused). */
    ensure_cache(6);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)14;
    cache[4] = (seq_t)33;
    cache[5] = (seq_t)70;
    cache[6] = (seq_t)157;
    computed_upto = 6;
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for indices < 1   */

    init_cache();

    if (n <= computed_upto)
        return cache[n];               /* already computed            */

    /* Grow the cache to hold the requested index. */
    ensure_cache(n);

    /* Compute missing terms using the recurrence. */
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        seq_t val = (seq_t)2 * cache[k - 1]
                  + (seq_t)3 * cache[k - 2]
                  - (seq_t)2 * cache[k - 3]
                  - (seq_t)6 * cache[k - 4]
                  - (seq_t)4 * cache[k - 5];
        cache[k] = val;
    }
    computed_upto = n;
    return cache[n];
}
