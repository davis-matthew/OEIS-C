/*
 * A266049  Number of n X 2 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors less than or equal
 *          to itself.
 * https://oeis.org/A266049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) + 7*a(n-3) - a(n-4), for n ≥ 5;
 *            initial terms: a(1)=3, a(2)=7, a(3)=35, a(4)=176
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function that returns the n-th term of OEIS sequence A266049,
   indexed with offset 1.  It memoises computed values so that repeated
   calls in any order are fast. */
seq_t a(idx_t n)
{
    /* Fixed initial values.  They are written with explicit cast to
       seq_t so that the constant is of the correct type. */
    static const seq_t init1 = (seq_t)3;
    static const seq_t init2 = (seq_t)7;
    static const seq_t init3 = (seq_t)35;
    static const seq_t init4 = (seq_t)176;

    /* Static cache of computed terms.  cache[0] is unused so that term
       n is stored at index n. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* highest index currently stored */

    /* Handle trivial cases directly.  If n == 0 we return 0, but the
       sequence is defined for n ≥ 1; this guarantees a defined behavior. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Ensure that the cache has space for at least the first four terms
       and is allocated.  This is done only on the very first call. */
    if (cache == NULL) {
        cache_len = 4;                     /* we will compute up to index 4 */
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL) {               /* allocation failure – return zero */
            return (seq_t)0;
        }
        cache[0] = 0;                      /* unused placeholder */
        cache[1] = init1;
        cache[2] = init2;
        cache[3] = init3;
        cache[4] = init4;
    }

    /* If n is already cached, return it immediately. */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Grow the cache to accommodate the new needed indices. */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         (new_len + 1) * sizeof(seq_t));
    if (new_cache == NULL) {                 /* reallocation failure */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute terms from cache_len + 1 up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                  + 7 * cache[i - 2]
                  + 7 * cache[i - 3]
                  -  cache[i - 4];
    }

    cache_len = n;   /* update maximum computed index */
    return cache[n];
}
