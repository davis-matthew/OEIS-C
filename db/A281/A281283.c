/*
 * A281283  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 350", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-4) - 16*a(n-8) for n > 23; initial terms: a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=13, a(4)=27, a(5)=52, a(6)=110, a(7)=211,
 *            a(8)=444, a(9)=834, a(10)=1773, a(11)=3391, a(12)=7104,
 *            a(13)=13346, a(14)=28373, a(15)=54063, a(16)=114136,
 *            a(17)=213542, a(18)=453977, a(19)=864935, a(20)=1826264, a(21...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[24] = {
    (seq_t)1,  (seq_t)3,  (seq_t)6,  (seq_t)13, (seq_t)27,
    (seq_t)52, (seq_t)110, (seq_t)211, (seq_t)444, (seq_t)834,
    (seq_t)1773, (seq_t)3391, (seq_t)7104, (seq_t)13346, (seq_t)28373,
    (seq_t)54063, (seq_t)114136, (seq_t)213542, (seq_t)453977,
    (seq_t)864935, (seq_t)1826264, (seq_t)3416614, (seq_t)7264089,
    (seq_t)13838503
};

static seq_t *cache = NULL;      /* array of already‑computed values */
static idx_t  cache_size = 0;    /* number of allocated slots */

seq_t a(idx_t n)
{
    /* Initialise cache with the first 24 terms on first call */
    if (cache == NULL) {
        cache_size = 24;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) { /* allocation failed, return 0 as safe fallback */
            return (seq_t)0;
        }
        for (idx_t i = 0; i < 24; ++i)
            cache[i] = init[i];
    }

    /* Extend cache if the requested index is beyond current capacity */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* reallocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)(17 * cache[i - 4] - 16 * cache[i - 8]);
        }
        cache_size = new_size;
    }

    return cache[n];
}
