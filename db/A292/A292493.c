/*
 * A292493  p-INVERT of the odd positive integers, where p(S) = 1 + S - 3 S^2.
 * https://oeis.org/A292493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 36, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS; A292493
 * Formula: a(0) = -1, a(1) = 1, a(2) = 12, a(3) = 25, a(4) = 61,
 * a(n) = 3*a(n-1) - 2*a(n-2) + 11*a(n-3) + a(n-4)  for n >= 5
 */

/* C implementation using memoisation and a simple recurrence */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;   /* highest index for which cache is valid */

    /* Handle the initial five terms directly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)61;

    /* Allocate the cache when first needed. */
    if (cache == NULL) {
        cache_size = (n >= 5) ? (n + 1) : 5;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)61;
        computed_up_to = 4;
    }

    /* Return the value if it is already computed. */
    if (n <= computed_up_to)
        return cache[n];

    /* Grow the cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] =   (3 * cache[i - 1])
                    - (2 * cache[i - 2])
                    + (11 * cache[i - 3])
                    + cache[i - 4];
    }
    computed_up_to = n;

    return cache[n];
}
