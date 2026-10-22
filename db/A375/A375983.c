/*
 * A375983  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 2, 4, or 5.
 * https://oeis.org/A375983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-6) + a(n-7) - a(n-10)
 *            for n ≥ 10 * a(0)=1, a(1)=2, a(2)=4, a(3)=6, a(4)=9, a(5)=12,
 *            a(6)=15, a(7)=20, a(8)=28, a(9)=40
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* number of subsets of {1,…,n} with no two elements differing by 2, 4, or 5 */
seq_t a(idx_t n)
{
    /* static cache and its state */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated capacity in elements */
    static idx_t   max    = (idx_t)-1; /* largest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* initial capacity: 16 elements (more than the 10 base values) */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        /* base values a(0) … a(9) */
        static const seq_t base[10] = {
            1, 2, 4, 6, 9, 12, 15, 20, 28, 40
        };
        for (size_t i = 0; i < 10; ++i)
            cache[i] = base[i];

        max = 9;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? (cap << 1) : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* return pre‑computed value if available */
    if (n <= max)
        return cache[(size_t)n];

    /* compute values from (max+1) up to n using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t val;
        if (i < 10) {
            /* should not occur because base already stored */
            static const seq_t base[10] = {
                1, 2, 4, 6, 9, 12, 15, 20, 28, 40
            };
            val = base[(size_t)i];
        } else {
            val = cache[(size_t)(i-1)]
                + cache[(size_t)(i-3)]
                - cache[(size_t)(i-4)]
                + cache[(size_t)(i-6)]
                + cache[(size_t)(i-7)]
                - cache[(size_t)(i-10)];
        }
        cache[(size_t)i] = val;
    }
    max = n;
    return cache[(size_t)n];
}
