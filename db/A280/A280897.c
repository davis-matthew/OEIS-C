/*
 * A280897  Number of n X 3 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 564*a(n-2) + 2072*a(n-3) + 4512*a(n-4) +
 *            2688*a(n-5) + 512*a(n-6) (for n > 8). Initial values (n = 1 ..
 *            8): 0, 10, 646, 22632, 610448, 14262832, 304584096, 6117000704
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static initial values for the first 8 terms of the sequence */
static const seq_t init[8] = {
    0,          /* a(1) */
    10,         /* a(2) */
    646,        /* a(3) */
    22632,      /* a(4) */
    610448,     /* a(5) */
    14262832,   /* a(6) */
    304584096,  /* a(7) */
    6117000704  /* a(8) */
};

/*
 * Compute a(n) for n >= 1.  The implementation uses the given
 * order‑6 recurrence with memoisation: once a term is computed it
 * is stored in a static cache so that subsequent calls can reuse it.
 */
seq_t a(idx_t n) {
    /* Return 0 for n == 0 (outside the OEIS offset).  This value
       is never used by the test harness. */
    if (n == 0) return 0;

    /* For the first 8 terms we can return the hard‑coded values. */
    if (n <= 8) return init[n - 1];

    /*--- Memoisation cache -------------------------------------*/
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cached_len = 0;     /* number of entries currently stored */
    static idx_t cached_cap = 0;     /* allocated capacity of the array */

    /* Grow the cache if necessary. */
    if (cache == NULL || cached_cap < n) {
        idx_t new_cap = n;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed – fallback */
            tmp = malloc(new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* cannot continue */
        }
        cache = (seq_t *)tmp;
        cached_cap = new_cap;
    }

    /* Initialise the first eight elements of the cache if not yet done. */
    if (cached_len < 8) {
        for (idx_t i = cached_len; i < 8; ++i) {
            cache[i] = init[i];
        }
        cached_len = 8;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t k = cached_len + 1; k <= n; ++k) {
        /* Compute a(k):
           a(k) = 42*a(k-1) - 564*a(k-2) + 2072*a(k-3)
                   + 4512*a(k-4) + 2688*a(k-5) + 512*a(k-6) */
        seq_t term = 42   * cache[k - 2] /* a(k-1) */
                     - 564  * cache[k - 3] /* a(k-2) */
                     + 2072 * cache[k - 4] /* a(k-3) */
                     + 4512 * cache[k - 5] /* a(k-4) */
                     + 2688 * cache[k - 6] /* a(k-5) */
                     + 512  * cache[k - 7];/* a(k-6) */
        cache[k - 1] = term;
    }

    cached_len = n;
    return cache[n - 1];
}
