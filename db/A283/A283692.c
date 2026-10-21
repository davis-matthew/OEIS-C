/*
 * A283692  Number of 2 X n 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 3*a(n-3) - 6*a(n-4) - 8*a(n-5)
 *            Initial values: a(1)=4, a(2)=16, a(3)=49, a(4)=161, a(5)=548
 *            This implementation uses memoisation to compute all terms up to
 *            the largest requested n efficiently in O(n) time
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t* cache   = NULL;     /* array of computed terms */
    static size_t  cap    = 0;        /* capacity of the array */
    static size_t  last   = (size_t)-1;/* index of last computed term (0‑based) */

    /* sanity check – the sequence is defined only for n >= 1 */
    if (n == 0) return 0;

    /* initialise cache with the first five values on the first call */
    if (last == (size_t)-1) {
        size_t need = 5;
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        cache[0] = 4;
        cache[1] = 16;
        cache[2] = 49;
        cache[3] = 161;
        cache[4] = 548;
        cap  = need;
        last = 4;          /* index of a(5) */
    }

    /* enlarge the cache if necessary */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : (size_t)n;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute missing terms up to the requested index */
    size_t idx = (size_t)n - 1;      /* zero‑based index for a(n) */
    if (idx > last) {
        for (size_t i = last + 1; i <= idx; ++i) {
            /* recurrence: a(i) = 3*a(i‑1) + a(i‑2) + 3*a(i‑3) − 6*a(i‑4) − 8*a(i‑5) */
            cache[i] = cache[i - 1] * 3
                     + cache[i - 2]
                     + cache[i - 3] * 3
                     - cache[i - 4] * 6
                     - cache[i - 5] * 8;
        }
        last = idx;
    }

    return cache[idx];
}
