/*
 * A328791  Triangular numbers of the form k^2 + 3.
 * https://oeis.org/A328791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3, a(2) = 28,
   a(n) = 34*a(n-1) - a(n-2) - 46  (n > 2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values (1‑based).  a(1) is at cache[0]. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated elements */
    static idx_t cache_size = 0;   /* number of stored terms */

    if (n < 1)
        return 0;   /* undefined for n < 1, but harmless */

    /* Return already computed value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Ensure the cache can accommodate n terms. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        if (new_cap < n)
            new_cap = (size_t)n;
        /* C/C++ compatible cast: */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if they are missing. */
    if (cache_size == 0) {
        cache[0] = (seq_t)3;
        cache_size = 1;
        if (n == 1)
            return cache[0];
    }
    if (cache_size == 1) {
        cache[1] = (seq_t)28;
        cache_size = 2;
        if (n == 2)
            return cache[1];
    }

    /* Prepare the last two computed values. */
    seq_t prev2 = cache[cache_size - 2];
    seq_t prev1 = cache[cache_size - 1];

    /* Compute missing terms up to n (inclusive). */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t curr = prev1 * (seq_t)34 - prev2 - (seq_t)46;
        cache[cache_size++] = curr;
        prev2 = prev1;
        prev1 = curr;
    }

    return cache[n - 1];
}
