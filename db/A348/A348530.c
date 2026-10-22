/*
 * A348530  a(1) = 1; a(n) = Sum_{k=1..ceiling(n/2)} a(k) * a(n-k).
 * https://oeis.org/A348530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 49, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A348530: a(1) = 1; a(n) = Σ_{k=1}^{⌈n/2⌉} a(k) * a(n-k)  (n≥1)
 * The function memoises computed values so that calls in any order or
 * repetition are handled efficiently.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n≥1. */
    if (n == 0) return 0;

    /* Memoisation cache: indices 0..n are stored in a dynamic array. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* number of allocated entries (>=1) */

    /* Grow the cache if it does not yet contain all indices up to n. */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp    = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */

        /* Initialise any newly created slots to 0. */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;

        cache = tmp;
        cap   = newcap;
    }

    /* Ensure the base case a(1)=1 is present. */
    cache[1] = 1;

    /* Compute values up to n that are still zero (not yet cached). */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue; /* already computed */

        seq_t sum = 0;
        idx_t m = (i >> 1) + (i & 1);          /* ceil(i/2) */
        for (idx_t k = 1; k <= m; ++k) {
            sum += cache[k] * cache[i - k];
        }
        cache[i] = sum;
    }

    return cache[n];
}
