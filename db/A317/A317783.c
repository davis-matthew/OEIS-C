/*
 * A317783  Number of equivalence classes of binary words of length n for the
 *          set of subwords {010, 101}.
 * https://oeis.org/A317783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-5) for n ≥ 5 Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=7
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..4 */
    static const seq_t init[5] = {1, 1, 1, 3, 7};

    /* Cache of previously computed values
       (indexed by 0 … cap-1). */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* number of stored terms */

    /* Initialise the cache with the first five terms on first call. */
    if (cap == 0) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) exit(1);     /* fatal: allocation failure */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cap = 5;
    }

    /* If the requested index is already cached, return it. */
    if (n < cap) return cache[(size_t)n];

    /* Grow the cache to hold indices up to n. */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);         /* fatal: allocation failure */
    cache = tmp;
    cap   = new_cap;

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t term = 2 * cache[(size_t)i - 1]    /* 2*a(i-1) */
                    - cache[(size_t)i - 2]      /* -a(i-2)   */
                    + cache[(size_t)i - 3]      /* +a(i-3)   */
                    + cache[(size_t)i - 5];     /* +a(i-5)   */
        cache[(size_t)i] = term;
    }

    return cache[(size_t)n];
}
