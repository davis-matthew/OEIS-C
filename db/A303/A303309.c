/*
 * A303309  Number of nX3 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) for n > 7 with initial values
 *            a(1) = 3, a(2) = 3, a(3) = 12, a(4) = 11, a(5) = 19, a(6) = 34,
 *            a(7) = 53
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms, index 1..cache_len */
static idx_t  cache_len = 0;     /* highest index for which cache[i] is defined */

/* Ensure that cache[1..n] are computed */
static void ensure(idx_t n)
{
    if (n <= cache_len) return;          /* nothing to do */

    /* allocate or grow the cache */
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }
    if (new_cache == NULL) {
        /* allocation failed – abort the process as no other recovery
           is possible in the required interface */
        abort();
    }
    cache = new_cache;

    /* compute new entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 7) {   /* initialise the first seven terms */
            switch ((int)i) {
            case 1: cache[i] = (seq_t)3; break;
            case 2: cache[i] = (seq_t)3; break;
            case 3: cache[i] = (seq_t)12;break;
            case 4: cache[i] = (seq_t)11;break;
            case 5: cache[i] = (seq_t)19;break;
            case 6: cache[i] = (seq_t)34;break;
            case 7: cache[i] = (seq_t)53;break;
            }
        } else {
            /* recurrence a(n) = a(n-1)+a(n-3)+a(n-4) */
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* value outside the defined offset */
    ensure(n);
    return cache[n];
}
