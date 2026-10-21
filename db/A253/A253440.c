/*
 * A253440  Number of (6+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n>5, with initial values
 *            a(1)=325, a(2)=324, a(3)=337, a(4)=372, a(5)=444. The function
 *            is memoised; it can be called in any order
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1; return 0. */
    if (n < 1)
        return (seq_t)0;

    /* --------------------------------------------------------------------
       Static cache: array of seq_t, dynamic size.
       last_computed stores the largest index for which cache[i] is known. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;   /* largest index with a computed value */

    /* First allocation: ensure at least space for indices 0..5. */
    if (cache == NULL) {
        size_t init = (size_t)(n + 1);
        if (init < 6) init = 6;                      /* at least 6 elements */
        cache = (seq_t *)calloc(init, sizeof(seq_t)); /* zero‑initialised */
        if (cache == NULL)
            return (seq_t)0;                         /* allocation failure */
        cache_cap = init;

        /* Initialise the known first five terms (indices 1..5). */
        cache[1] = (seq_t)325;
        cache[2] = (seq_t)324;
        cache[3] = (seq_t)337;
        cache[4] = (seq_t)372;
        cache[5] = (seq_t)444;

        /* cache[0] stays 0 – unused. */
        last_computed = (idx_t)5;
    }

    /* Grow the cache if the requested index is out of current bounds. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence (valid for n>5). */
    if (n > last_computed) {
        idx_t start = last_computed + 1;
        if (start <= 5) start = 6;                  /* recurrence starts at 6 */
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - 2*a(i-2) */
            seq_t term1 = (seq_t)3 * cache[i - 1];
            seq_t term2 = (seq_t)2 * cache[i - 2];
            cache[i] = term1 - term2;
        }
        last_computed = n;
    }

    return cache[n];
}
