/*
 * A291234  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3 -
 *          S^4.
 * https://oeis.org/A291234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 2*a(n-3) - 7*a(n-4) + 2*a(n-5) +
 *            5*a(n-6) - a(n-7) - a(n-8); Initial values: a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=12, a(4)=28, a(5)=67, a(6)=156, a(7)=370, a(8)=866
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity in elements  */
    static idx_t  len   = 0;     /* number of elements currently computed */

    /* Initialise the cache with the first nine terms if not already done */
    if (len == 0) {
        size_t need = 9;
        if (cap < need) {
            cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
            cap   = need;
        }
        cache[0] = 1;  cache[1] = 2;  cache[2] = 5;  cache[3] = 12;
        cache[4] = 28; cache[5] = 67; cache[6] = 156; cache[7] = 370;
        cache[8] = 866;
        len = 9;
    }

    /* If the requested index is not yet computed, extend the cache */
    if ((idx_t)n >= len) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            /* grow capacity to at least need (doubling strategy) */
            size_t new_cap = cap ? cap : 1;
            while (new_cap < need) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }
        for (idx_t i = len; i <= n; ++i) {
            idx_t i1 = i - 1, i2 = i - 2, i3 = i - 3, i4 = i - 4;
            idx_t i5 = i - 5, i6 = i - 6, i7 = i - 7, i8 = i - 8;

            /* compute using seq_t arithmetic; the recurrence is guaranteed
               to produce non‑negative results for all n, so unsigned overflow
               is not a problem for values that fit in the type. */
            cache[(size_t)i] =
                cache[(size_t)i1] + 5 * cache[(size_t)i2]
                - 2 * cache[(size_t)i3] - 7 * cache[(size_t)i4]
                + 2 * cache[(size_t)i5] + 5 * cache[(size_t)i6]
                - cache[(size_t)i7] - cache[(size_t)i8];
        }
        len = n + 1;
    }

    return cache[(size_t)n];
}
