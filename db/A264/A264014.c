/*
 * A264014  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,2.
 * https://oeis.org/A264014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 25*a(n-2) for n > 4 initial values: a(1)=8,
 *            a(2)=105, a(3)=1573, a(4)=20570
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values, indexed by n starting at 1 */
    static seq_t *cache = NULL;
    static size_t cached = 0; /* highest n computed so far */

    if (n < (idx_t)1) {
        return 0; /* offset: a(1) is first term */
    }

    size_t required = (size_t)n + 1;   /* need index up to n */
    if (required > cached) {
        /* grow the cache */
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 as a fall‑back */
            return 0;
        }
        cache = tmp;

        /* compute new entries */
        for (size_t i = cached + 1; i < required; ++i) {
            if (i <= 4) {
                switch (i) {
                    case 1: cache[i] = 8;                  break;
                    case 2: cache[i] = 105;                break;
                    case 3: cache[i] = 1573;               break;
                    case 4: cache[i] = 20570;              break;
                }
            } else {
                /* recurrence: a(i) = 15*a(i-1) - 25*a(i-2) */
                cache[i] = ((seq_t)15 * cache[i-1]) - ((seq_t)25 * cache[i-2]);
            }
        }
        cached = required - 1;
    }

    return cache[(size_t)n];
}
