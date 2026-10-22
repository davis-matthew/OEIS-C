/*
 * A302658  Number of minimal total dominating sets in the wheel graph on n
 *          nodes.
 * https://oeis.org/A302658
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A300738(n-1) + (n-1)
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the linear recurrence */
/*   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) - a(n-7) + a(n-9) + a(n-10) - a(n-11)   */
/* for all n >= 13, with the initial values   */
/*   a(2)=1, a(3)=2, a(4)=6, a(5)=8, a(6)=10, a(7)=15, a(8)=14, a(9)=12, a(10)=21, a(11)=35, a(12)=33. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure cache is initialized to hold at least indices up to n */
    if (cache == NULL) {
        /* Start with some generous capacity */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure: return 0 */
        /* initialise to zero */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* base values */
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 6;
        cache[5] = 8;
        cache[6] = 10;
        cache[7] = 15;
        cache[8] = 14;
        cache[9] = 12;
        cache[10] = 21;
        cache[11] = 35;
        cache[12] = 33;
        cache_len = 12;
    }

    /* Expand cache if necessary */
    if (n > cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;           /* double until big enough */

        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            cache = tmp;
            /* initialise new slots to zero */
            for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k <= 12) continue;          /* base values already set */

            seq_t term = cache[(size_t)(k - 1)];            /* 2*a(k-1) */
            term += cache[(size_t)(k - 1)];
            term -= cache[(size_t)(k - 2)];
            term += cache[(size_t)(k - 3)];
            term -= cache[(size_t)(k - 4)];
            term -= cache[(size_t)(k - 7)];
            term += cache[(size_t)(k - 9)];
            term += cache[(size_t)(k -10)];
            term -= cache[(size_t)(k -11)];

            cache[(size_t)k] = term;
        }
        cache_len = n;
    }

    if (n <= cache_len)
        return cache[(size_t)n];
    return 0;   /* for n < 2, sequence is defined only for n >= 2 */
}
