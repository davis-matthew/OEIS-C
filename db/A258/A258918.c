/*
 * A258918  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum equal to the antidiagonal sum or central row sum
 *          equal to the central column sum.
 * https://oeis.org/A258918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 18*a(n-3) Initial values: a(1)=200,
 *            a(2)=576, a(3)=1680
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build */
static seq_t *cache = NULL;   /* memoised terms, 0‑based index */
static idx_t cached = 0;      /* number of terms stored */

/* Expand the cache to at least 'needed' terms and fill any missing
   values up to that term. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cached)
        return;

    /* Figure out new capacity */
    size_t new_cap = (size_t)needed;
    if (cache == NULL)
    {
        /* Ensure space for the first three base cases */
        if (new_cap < 3)
            new_cap = 3;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    }
    else
    {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }

    /* Set base cases if not already stored */
    if (cached < 1 && new_cap >= 1)
        cache[0] = 200;          /* a(1) */
    if (cached < 2 && new_cap >= 2)
        cache[1] = 576;          /* a(2) */
    if (cached < 3 && new_cap >= 3)
        cache[2] = 1680;         /* a(3) */

    if (cached < 3)
        cached = 3;

    /* Compute remaining terms up to 'needed' */
    for (idx_t i = cached + 1; i <= needed; ++i)
    {
        /* i is 1‑based; array offset is i-1 */
        seq_t term = 3 * cache[i - 2] + 18 * cache[i - 4] - 6 * cache[i - 3];
        cache[i - 1] = term;
    }

    cached = needed;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, but return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
