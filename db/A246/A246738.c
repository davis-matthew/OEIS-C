/*
 * A246738  Number of length 1+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 85, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 3*a(n-1) - 8*a(n-3) + 6*a(n-4) + 6*a(n-5) - 8*a(n-6) + 3*a(n-8) - a(n-9).
   Initial values: a(1)=2, a(2)=12, a(3)=124, a(4)=424, a(5)=1566,
   a(6)=3876, a(7)=9368, a(8)=18768, a(9)=36250. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_size = 0;        /* maximum index stored (at least 9 after init) */

/* constant initial values (index 0 unused) */
static const seq_t init_vals[10] = {
    0,      /* unused */
    2,      /* a(1) */
    12,     /* a(2) */
    124,    /* a(3) */
    424,    /* a(4) */
    1566,   /* a(5) */
    3876,   /* a(6) */
    9368,   /* a(7) */
    18768,  /* a(8) */
    36250   /* a(9) */
};

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate indices 0..9 */
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        for (size_t i = 1; i <= 9; ++i)
            cache[i] = init_vals[i];
        cache_size = 9;
    }

    /* extend cache if necessary */
    if ((size_t)n > cache_size) {
        size_t limit = (size_t)n;
        cache = (seq_t *)realloc(cache, (limit + 1) * sizeof(seq_t));
        for (size_t i = cache_size + 1; i <= limit; ++i) {
            /* use the recurrence to compute a(i) */
            seq_t pos = cache[i - 1] * 3
                       + cache[i - 4] * 6
                       + cache[i - 5] * 6
                       + cache[i - 8] * 3;

            seq_t neg = cache[i - 3] * 8
                       + cache[i - 6] * 8
                       + cache[i - 9];

            cache[i] = pos - neg;   /* pos >= neg for all i >= 10 */
        }
        cache_size = limit;
    }

    return cache[(size_t)n];
}
