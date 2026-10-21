/*
 * A291395  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - 2 S)(1 - 3 S).
 * https://oeis.org/A291395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 12*a(n-3) - 6*a(n-4) for n >= 5, a(0)
 *            = 5, a(1) = 24, a(2) = 103, a(3) = 425, a(4) = 1704
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291395 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cache[0 .. computed_up_to-1] */
static idx_t  cache_cap   = 0;   /* allocated size of cache */
static idx_t  computed_up_to = 0; /* first index not yet computed */

/* Ensure that cache has space up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;

    /* realloc may fail; abort on failure as per build toolkit. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* keep the original cache intact on error */

    /* Initialise any new slots to zero (default constructor or literal). */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the known base terms; run only once. */
static void initialise_base(void)
{
    if (computed_up_to != 0)
        return;

    ensure_cache(4);           /* need indices 0..4 */

    cache[0] = 5;
    cache[1] = 24;
    cache[2] = 103;
    cache[3] = 425;
    cache[4] = 1704;

    computed_up_to = 5;        /* first not yet computed index */
}

/* Compute a(n) using the recurrence and cache intermediate results. */
seq_t a(idx_t n)
{
    initialise_base();          /* make sure base terms exist */
    ensure_cache(n);            /* guarantee room for index n */

    if (n < computed_up_to)
        return cache[n];       /* already computed */

    /* Compute any missing values up to n. */
    for (idx_t k = computed_up_to; k <= n; ++k)
    {
        /* k >= 5 because computed_up_to == 5 after base init. */
        seq_t v1 = cache[k-1];   /* a(k-1) */
        seq_t v2 = cache[k-2];   /* a(k-2) */
        seq_t v3 = cache[k-3];   /* a(k-3) */
        seq_t v4 = cache[k-4];   /* a(k-4) */

        seq_t val = (seq_t)5 * v1 - v2 - (seq_t)12 * v3 - (seq_t)6 * v4;
        cache[k] = val;
    }

    computed_up_to = n + 1;     /* next missing index */
    return cache[n];
}
