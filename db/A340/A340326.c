/*
 * A340326  a(n) = a(n-2) + (-1)^n*a(n-1) + n*(1-(-1)^n) with a(0) = a(1) = 1.
 * https://oeis.org/A340326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + (-1)^n*a(n-1) + n*(1-(-1)^n), a(0)=a(1)=1
 *            Recurrence: if n even -> a(n)=a(n-2)+a(n-1) if n odd ->
 *            a(n)=a(n-2)-a(n-1)+2*n The implementation memoizes results so
 *            calls in arbitrary order remain fast
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* a(n) expects seq_t and idx_t to be defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array holding computed terms */
    static idx_t  cache_size = 0;    /* current allocated size */
    static idx_t  computed_up_to = 1;/* last index for which a value is computed */

    /* initial allocation: a(0)=a(1)=1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* grow the allocated array if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0;  /* allocation failed – fallback to 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* if the requested term is already computed, return it */
    if (n <= computed_up_to)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val;
        if ((i & 1) == 0) { /* even i: a[i] = a[i-2] + a[i-1] */
            val = cache[i - 2] + cache[i - 1];
        } else { /* odd i: a[i] = a[i-2] - a[i-1] + 2*i */
            /* use signed intermediate to avoid unsigned underflow */
            long long diff = (long long)cache[i - 2]
                           - (long long)cache[i - 1]
                           + (long long)i * 2LL;
            val = (seq_t)diff;
        }
        cache[i] = val;
    }

    computed_up_to = n;
    return cache[n];
}
