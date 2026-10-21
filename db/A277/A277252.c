/*
 * A277252  a(n) = a(n-2) + a(n-3) + a(n-4) with a(0) = 0, a(1) = a(2) = 1,
 *          a(3) = 0.
 * https://oeis.org/A277252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4), * a(0) = 0, a(1) = 1, a(2) = 1,
 *            a(3) = 0
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                    /* for size_t */
#include "../utils/seq_realloc.h"      /* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *cache = NULL;            /* memoisation array */
static idx_t  cache_capacity = 0;      /* allocated number of elements */
static idx_t  computed_len  = 0;        /* elements that have already been computed */

/* Return the n‑th term of A277252 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* Allocate / grow the cache to hold indices up to n */
    if ((cache == NULL) || (cache_capacity < (size_t)(n + 1))) {
        seq_t *tmp = seq_realloc(cache,
                                 cache_capacity,
                                 (size_t)(n + 1));
        if (tmp == NULL) {            /* allocation failure – abort */
            abort();
        }
        cache          = tmp;
        cache_capacity = (size_t)(n + 1);
    }

    /* Initialise the first four entries if they are not yet set */
    if (computed_len < 4) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        computed_len = 4;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_len; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 3] + cache[i - 4];
    }
    computed_len = n + 1;

    return cache[n];
}
