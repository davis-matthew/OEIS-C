/*
 * A274110  Number of equivalence classes of ballot paths of length n for the
 *          string uu.
 * https://oeis.org/A274110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A274110 – Number of equivalence classes of ballot paths of length n for the string uu
 * Recurrence: a(n) = 2*a(n-1) - a(n-3) + a(n-4) - a(n-5)
 * Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for computed terms */
static seq_t *cache = NULL;          /* array indexed from 0, a(n) stored at cache[n] */
static size_t cache_cap = 0;         /* allocated capacity (max index + 1) */
static idx_t computed_up_to = 0;     /* largest n for which a(n) is stored */

/* Initialise cache with base values up to at least n */
static void init_cache(idx_t n)
{
    idx_t needed = (idx_t)5;
    if (n > needed) needed = n;
    size_t cap = (size_t)needed + 1;      /* include index 0 unused, indices 1..needed */
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) abort();
    cache_cap = cap;
    memset(cache, 0, cap * sizeof(seq_t));

    /* Base values (index 1..5) */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)5;
    cache[5] = (seq_t)8;

    computed_up_to = 5;   /* all base terms are now present */
}

/* Ensure that terms up to n are computed and stored in the cache */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        init_cache(n);
        return;
    }

    if (n <= computed_up_to) return;

    /* Grow the allocated buffer if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t term = (seq_t)2 * cache[i - 1];
        term -= cache[i - 3];
        term += cache[i - 4];
        term -= cache[i - 5];
        cache[i] = term;
    }
    computed_up_to = n;
}

/* Public function that returns the n-th term (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but never called by tests */
    ensure_capacity(n);
    return cache[n];
}
