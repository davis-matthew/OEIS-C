/*
 * A302130  a(n) = a(a(n-3)) + a(n-a(n-2)) with a(1) = a(2) = a(3) = a(4) = 1,
 *          a(5) = 2, a(6) = 5.
 * https://oeis.org/A302130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-3)) + a(n-a(n-2)), a(1)=a(2)=a(3)=a(4)=1, a(5)=2,
 *            a(6)=5 Linear recurrence: a(n) = 2*a(n-3) - a(n-6) for n > 12
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of computed terms; cache[0] unused */
static idx_t  cache_cap = 0;    /* allocated size (≥ cache_max+1)                */
static idx_t  cache_max = 0;    /* largest index that has been calculated        */

/* Resize the cache array so that it can hold a(n) */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)  /* allocation failed */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the first six terms.  */
static void init_cache(void)
{
    ensure_cache_size(6);

    cache[0] = 0;          /* index 0 is unused */
    cache[1] = 1;
    cache[2] = 1;
    cache[3] = 1;
    cache[4] = 1;
    cache[5] = 2;
    cache[6] = 5;
    cache_max = 6;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below the offset is irrelevant */
        return 0;

    /* Make sure the cache can contain a[n] */
    ensure_cache_size(n);

    /* Initialise the first terms once */
    if (cache_max == 0)
        init_cache();

    /* Compute all missing terms up to n */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 12) { /* use the original recurrence for n ≤ 12 */
                /* a(i) = a(a(i-3)) + a(i-a(i-2)) */
                idx_t idx1 = (idx_t)cache[i - 3];
                seq_t  val1 = cache[idx1];

                idx_t idx2 = i - (idx_t)cache[i - 2];
                seq_t  val2 = cache[idx2];

                cache[i] = val1 + val2;
            } else {     /* linear recurrence for n > 12 */
                cache[i] = 2 * cache[i - 3] - cache[i - 6];
            }
        }
        cache_max = n;
    }

    return cache[n];
}
