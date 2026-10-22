/*
 * A313730  Coordination sequence Gal.6.234.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 with initial
 *            values: a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=20, a(5)=25,
 *            a(6)=29, a(7)=34, a(8)=39, a(9)=44, a(10)=49, a(11)=54, a(12)=59
 *            This implementation memoizes results so calls can be in any
 *            order. The array is grown as needed up to the maximum...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 5, 10, 15, 20, 25, 29, 34, 39, 44, 49, 54, 59
};

static seq_t *cache = NULL;       /* memoization array */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t max_computed = 0; /* largest n for which cache[n] is valid */

/* Return the nth term of sequence A313730 */
seq_t a(idx_t n)
{
    /* Bounds that are less than known initial terms */
    if (n < 13)
        return init_vals[n];

    /* First use: allocate and initialise cache with initial values */
    if (cache == NULL) {
        cache_cap = (size_t) (n + 1 > 13 ? n + 1 : 13);
        cache = (seq_t *) malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        max_computed = 12;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = cache_cap;
        while ((size_t)n + 1 > newcap)
            newcap <<= 1;               /* double until large enough */
        seq_t *tmp = (seq_t *) realloc(cache, newcap * sizeof(seq_t));
        if (tmp)
            cache = tmp;
        cache_cap = newcap;
    }

    /* Compute missing values up to n (inclusive) */
    if (n > max_computed) {
        for (uint64_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
        max_computed = n;
    }

    return cache[n];
}
