/*
 * A366325  G.f. satisfies A(x) = (1 + x) * (1 + x/A(x)).
 * https://oeis.org/A366325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) = -3*(2*n - 3)*a(n-1) - 5*(n - 3)*a(n-2);
   initial values: a(0)=1, a(1)=2, a(2)=-1
   This recurrence holds for n>=3. */
#include <stdlib.h>

/* static cache to allow arbitrary order of calls */
static seq_t *cache = NULL;      /* table of previously computed terms */
static idx_t cache_len = 0;      /* number of entries currently valid   */
static idx_t cache_cap = 0;      /* allocated capacity                   */

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;   /* nothing to do, already computed */

    /* enlarge the cache if necessary */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)-1;
        else {
            seq_t term = (seq_t)-3 * (seq_t)(2 * i - 3) * cache[i - 1]
                       - (seq_t)5 * (seq_t)(i - 3) * cache[i - 2];
            cache[i] = term / (seq_t)i;  /* integer division is exact */
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
