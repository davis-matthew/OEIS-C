/*
 * A245031  Numbers m such that 3*m+1 and 8*m+1 are both squares.
 * https://oeis.org/A245031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245031: Numbers m such that 3*m+1 and 8*m+1 are both squares
 *
 * Recurrence (valid for n ≥ 6):
 *    a(n) = a(n-1) + 98*a(n-2) – 98*a(n-3) – a(n-4) + a(n-5)
 *
 * Initial values:
 *    a(1) = 0
 *    a(2) = 1
 *    a(3) = 21
 *    a(4) = 120
 *    a(5) = 2080
 *
 * The function memoises computed terms and keeps them in a static
 * dynamically‑allocated array.  It works for any seq_t / idx_t
 * combinations that satisfy the constraints in the problem statement.
 */

#include <stdlib.h>  /* malloc, realloc, free */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    /* Handle the trivial out‑of‑range case. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: 1‑based indexing, cache[0] unused. */
    static seq_t *cache   = NULL;  /* pointer to cached values */
    static idx_t  cache_size = 0;  /* capacity in terms of indices (0…size-1) */
    static idx_t  computed   = 0;  /* highest index that has been computed */

    /* Ensure the cache can hold indices up to n. */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);          /* +1 for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – cannot proceed.  Return 0 (unspecified). */
            free(cache);
            cache   = NULL;
            cache_size = 0;
            computed   = 0;
            return (seq_t)0;
        }
        cache      = tmp;
        cache_size = new_size;
    }

    /* Initialise first five terms once. */
    if (computed < 5) {
        /* Ensure capacity for at least indices 0…5 (six slots). */
        if (cache_size < 6) {
            size_t new_size = 6;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache      = NULL;
                cache_size = 0;
                computed   = 0;
                return (seq_t)0;
            }
            cache      = tmp;
            cache_size = new_size;
        }
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)21;
        cache[4] = (seq_t)120;
        cache[5] = (seq_t)2080;
        computed = 5;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)98 * cache[i-2]
                 - (seq_t)98 * cache[i-3]
                 - cache[i-4]
                 + cache[i-5];
    }

    computed = n;
    return cache[n];
}
