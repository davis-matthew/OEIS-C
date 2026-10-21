/*
 * A272853  Ramanujan's alpha-series.
 * https://oeis.org/A272853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3) with a(0)=9, a(1)=791,
 *            a(2)=65601 (derived from the recurrence using the initial values
 *            a(-3)=-11161, a(-2)=-135, a(-1)=-1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values a(0)..a(m) */
static idx_t       cached = 0;/* number of cached entries  */

/* Extend the cache up to index n (inclusive).  The routine uses
   the already stored values for lower indices.  */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= cached)
        return;                     /* already stored */

    /* Allocate or grow the cache to hold indices 0..n. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* allocation failure */

    /* If this is a fresh allocation, initialise the first three
       elements with the known values, then compute the rest. */
    if (cached == 0) {
        if (new_cap > 0) tmp[0] = 9;
        if (new_cap > 1) tmp[1] = 791;
        if (new_cap > 2) tmp[2] = 65601;
        /* compute from index 3 up to n */
        for (idx_t i = 3; i <= n; ++i)
            tmp[i] = 82 * tmp[i - 1] + 82 * tmp[i - 2] - tmp[i - 3];
    } else {
        /* Extend from the previously cached last index. */
        idx_t start = cached;       /* first index not yet computed */
        for (idx_t i = start; i <= n; ++i)
            tmp[i] = 82 * tmp[i - 1] + 82 * tmp[i - 2] - tmp[i - 3];
    }

    cache = tmp;
    cached = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    /* Small indices can be answered immediately. */
    if (n < 3) {
        static const seq_t init[3] = {9, 791, 65601};
        return init[n];
    }

    /* Ensure the cache contains a(n). */
    if (!cache || cached <= n)
        ensure_capacity(n);

    return cache[n];
}
