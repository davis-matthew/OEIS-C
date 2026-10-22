/*
 * A304723  a(n) = 5^(n-1)*(3^n - 1)/2.
 * https://oeis.org/A304723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1)*(3^n - 1)/2 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=20*a(n-1)-75*a(n-2) (n>1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values up to the largest requested n */
    static seq_t *cache = NULL;
    static size_t max_n = 0;   /* largest n already computed */
    static size_t cap = 0;     /* allocated capacity of cache */

    /* Ensure cache covers index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* Out of memory: fall back to zero (not expected) */
        cache = tmp;
        cap = new_cap;
    }

    /* Extend computation up to n if needed */
    if ((size_t)n > max_n) {
        idx_t i;
        /* Initialise first two terms if not already done */
        if (max_n == 0) {
            cache[0] = 0;
            max_n = 0;
        }
        if (max_n < 1) {
            cache[1] = 1;
            max_n = 1;
        }
        for (i = (idx_t)(max_n + 1); i <= n; ++i) {
            /* recurrence a(i) = 20*a(i-1) - 75*a(i-2) */
            seq_t t1 = 20 * cache[i - 1];
            seq_t t2 = 75 * cache[i - 2];
            cache[i] = t1 - t2;
        }
        max_n = (size_t)n;
    }

    return cache[n];
}
