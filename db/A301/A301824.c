/*
 * A301824  Number of 3Xn 0..1 arrays with every element equal to 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 18 * a(n) = 2*a(n-1) + 6*a(n-2) - a(n-3) - 3*a(n-4) +
 *            4*a(n-5) * - 4*a(n-6) - 32*a(n-7) - 9*a(n-8) - 4*a(n-9) -
 *            19*a(n-10) * + 17*a(n-11) + 3*a(n-12) + a(n-13) - a(n-14) * *
 *            Initial values: * a(1)=0, a(2)=5, a(3)=4, a(4)=25, a(5)=74,
 *            a(6)=278, a(7)=968, * a(8)=3450, a(9)=12210, a(10)=4331...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base_vals[18] = {
    0ULL,      /* a(1) */
    5ULL,      /* a(2) */
    4ULL,      /* a(3) */
    25ULL,     /* a(4) */
    74ULL,     /* a(5) */
    278ULL,    /* a(6) */
    968ULL,    /* a(7) */
    3450ULL,   /* a(8) */
    12210ULL,  /* a(9) */
    43316ULL,  /* a(10) */
    153582ULL, /* a(11) */
    544554ULL, /* a(12) */
    1930920ULL,/* a(13) */
    6846494ULL,/* a(14) */
    24276442ULL,/* a(15) */
    86078740ULL,/* a(16) */
    305217538ULL,/* a(17) */
    1082235538ULL /* a(18) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined, guard */
    if (n <= 18) return _base_vals[n-1];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of allocated slots (index 0..size-1) */
    static idx_t computed = 18;   /* highest index that has been filled */

    /* Ensure cache can hold indices up to n (we ignore index 0) */
    idx_t needed = n + 1;
    if (cache_size < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure, unlikely in tests */
        cache = new_cache;
        cache_size = needed;
        /* Initialise base values in the new cache area if not already present */
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = _base_vals[i-1];
    }

    /* Compute any missing values up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val = 2 * cache[k-1];
        val += 6 * cache[k-2];
        val -= cache[k-3];
        val -= 3 * cache[k-4];
        val += 4 * cache[k-5];
        val -= 4 * cache[k-6];
        val -= 32 * cache[k-7];
        val -= 9 * cache[k-8];
        val -= 4 * cache[k-9];
        val -= 19 * cache[k-10];
        val += 17 * cache[k-11];
        val += 3 * cache[k-12];
        val += cache[k-13];
        val -= cache[k-14];
        cache[k] = val;
    }
    computed = n;
    return cache[n];
}
