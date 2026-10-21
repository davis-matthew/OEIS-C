/*
 * A287643  a(1) = ... = a(6) = 1; a(n) = a(a(n-3)) + a(n-a(n-6)) for n > 6.
 * https://oeis.org/A287643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-3)) + a(n-a(n-6)), a(1)=…=a(6)=1 Offset: 1 (a(1) is
 *            the first term)
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache      = NULL;   /* 1‑based array of values */
static size_t cache_cap  = 0;      /* allocated capacity (including index 0) */
static idx_t  max_used   = 0;      /* largest n computed so far */

/* Grow the cache to be able to index at least 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    seq_t *tmp = (cache == NULL) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                 : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for 0, return 0 safely */
        return (seq_t)0;

    ensure_capacity(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = max_used + 1; i <= n; ++i)
    {
        if (i <= 6)                /* initial fixed values */
        {
            cache[i] = (seq_t)1;
            continue;
        }

        idx_t idx1 = (idx_t)cache[i - 3];          /* a(i-3) */
        idx_t idx2 = i - (idx_t)cache[i - 6];      /* i - a(i-6) */

        /* The recurrence guarantees idx1, idx2 are in [1, i-1]. */
        assert(idx1 > 0 && idx2 > 0 && idx1 < i && idx2 < i);

        cache[i] = cache[idx1] + cache[idx2];
    }

    if (n > max_used)
        max_used = n;

    return cache[n];
}
