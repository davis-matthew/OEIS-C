/*
 * A305583  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 12*a(n-3) - 9*a(n-5) - 27*a(n-6) (for
 *            n > 10; a(1) = 16, a(2) = 8, a(3) = 19, a(4) = 38, a(5) = 76,
 *            a(6) = 327, a(7) = 967, a(8) = 2641, a(9) = 8578, a(10) = 26437)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base_vals[11] = {
    0,          /* index 0 unused */
    (seq_t)16,  /* a(1) */
    (seq_t)8,   /* a(2) */
    (seq_t)19,  /* a(3) */
    (seq_t)38,  /* a(4) */
    (seq_t)76,  /* a(5) */
    (seq_t)327, /* a(6) */
    (seq_t)967, /* a(7) */
    (seq_t)2641,/* a(8) */
    (seq_t)8578,/* a(9) */
    (seq_t)26437/* a(10) */
};

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* capacity of the cache array */
    static idx_t   last  = 0;   /* highest index currently computed */

    if (n == 0) return 0;      /* undefined, but safe guard */

    /* ensure cache is large enough to store up to index n */
    if (cap <= (size_t)n) {
        size_t newcap = cap ? cap : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {             /* allocation failure */
            cache = NULL;
            cap = 0;
            return 0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* initialise base values first time through (if not already done) */
    if (last == 0) {
        cache[0] = 0;
        for (idx_t i = 1; i <= 10 && i <= (idx_t)cap; ++i)
            cache[i] = base_vals[i];
        last = 10;
    }

    /* if requested value already computed, return it */
    if (n <= last) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = cache[i-1] 
                     + (seq_t)3 * cache[i-2] 
                     + (seq_t)12 * cache[i-3]
                     - (seq_t)9 * cache[i-5]
                     - (seq_t)27 * cache[i-6];
        }
    }
    last = n;
    return cache[n];
}
