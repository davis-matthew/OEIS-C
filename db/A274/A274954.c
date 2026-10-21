/*
 * A274954  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-1,-1) (-2,0) or (0,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3) + a(n-4) for n>5, with a(1)=3,
 *            a(2)=24, a(3)=90, a(4)=384, a(5)=1638
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based, cache[0] unused */
    static seq_t *cache = NULL;   /* array of values */
    static size_t cap   = 0;      /* allocated capacity (in seq_t elements) */
    static size_t len   = 0;      /* highest index computed (incl. base terms) */

    /* base values */
    static const seq_t base[6] = {0, 3, 24, 90, 384, 1638};  /* base[0] unused */

    if (n == 0) return 0;              /* offset safety – not used by OEIS */

    /* initialise cache if first call */
    if (cache == NULL) {
        /* start with a small capacity and grow as needed */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – abort */
            return 0;
        }
        /* fill base terms */
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
        cache[5] = base[5];
        len = 5;
    }

    /* extend cache if necessary */
    size_t idx = (size_t)n;
    if (idx >= cap) {
        size_t newcap = cap;
        while (newcap <= idx) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure – abort */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms using recurrence */
    if (idx > len) {
        for (size_t i = len + 1; i <= idx; ++i) {
            cache[i] = 4 * cache[i - 1] + 4 * cache[i - 3] + cache[i - 4];
        }
        len = idx;
    }

    return cache[idx];
}
