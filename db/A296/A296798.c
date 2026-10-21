/*
 * A296798  Number of n X 2 0..1 arrays with each 1 adjacent to 1, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A296798.
   Empirical recurrence:
     a(1)=2, a(2)=8, a(3)=15, a(4)=41, a(5)=122, a(6)=308
     a(n) = a(n-1) + 3*a(n-2) + 9*a(n-3)
            - 4*a(n-4) - 12*a(n-5) - 16*a(n-6)   for n ≥ 7
   The function memoises computed terms for fast repeated access. */
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;   /* array of a(1),a(2),...  */
static size_t size  = 0;      /* number of entries currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */

    size_t idx = n - 1;              /* cache index of a(n) */

    if (idx >= size) {               /* need to compute up to n */
        ensure_cache(idx + 1, &cache, &size);

        /* initialise base values – safe to overwrite after re‑allocation */
        if (size > 0) cache[0] = 2;
        if (size > 1) cache[1] = 8;
        if (size > 2) cache[2] = 15;
        if (size > 3) cache[3] = 41;
        if (size > 4) cache[4] = 122;
        if (size > 5) cache[5] = 308;

        /* compute missing terms */
        for (size_t i = 6; i <= idx; ++i) {
            seq_t val = cache[i-1]            /* a(n-1) */
                      + 3 * cache[i-2]        /* 3*a(n-2) */
                      + 9 * cache[i-3]        /* 9*a(n-3) */
                      - 4 * cache[i-4]        /* -4*a(n-4) */
                      - 12 * cache[i-5]       /* -12*a(n-5) */
                      - 16 * cache[i-6];      /* -16*a(n-6) */
            cache[i] = val;
        }
        size = idx + 1;  /* update current size */
    }

    return cache[idx];
}
