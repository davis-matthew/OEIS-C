/*
 * A329257  a(n) is the number of numbers k such that a(n-2*k) = a(n-k) and 0
 *          < n-2*k < n-k < n .
 * https://oeis.org/A329257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{ k : a(n-2k) = a(n-k) and 0 < n-2k < n-k < n }
 * Range:     u8 n <= 9601, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache array holding already-computed values, indexed by n.
       cache[0] is unused but initialized to 0 to keep bounds tidy. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* largest n stored in cache */

    if (n == 0) {
        /* Off‑by‑one safety; the OEIS offset starts at 1. */
        return 0;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if (n > cache_size) {
        size_t new_len = (size_t)n + 1;  /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Initialise the unused slot 0 */
        if (cache_size == 0) {
            cache[0] = 0;
        }

        /* Compute all new values sequentially. */
        for (idx_t m = cache_size + 1; m <= n; ++m) {
            seq_t cnt = 0;
            for (idx_t k = 1; 2 * k < m; ++k) {
                if (cache[m - 2 * k] == cache[m - k]) {
                    ++cnt;
                }
            }
            cache[m] = cnt;
        }
        cache_size = n;
    }

    return cache[n];
}
