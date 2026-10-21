/*
 * A283504  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 641", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0. For n>3: a(n)=10*a(n-1)+a(n-2)-10*a(n-3). (The
 *            first four terms are 1,0,101,1100.)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Utilities used  */
#include "../utils/pow10.h"
#include "../utils/seq_alloc_new_array.h"

/*  static cache for memoisation  */
static seq_t *cache        = NULL;   /* array of computed terms          */
static idx_t cache_len     = 0;      /* number of terms stored (>=0)     */
static idx_t cache_cap     = 0;      /* allocated size (>=cache_len)     */

/*  Allocate or grow the cache to hold indices [0..need]  */
static void ensure_capacity(idx_t need)
{
    if (need + 1 <= cache_cap)   /* enough space already  */
        return;

    /* allocate new array */
    seq_t *new_cache = seq_alloc_new_array((size_t)(need + 1));
    if (!new_cache)             /* allocation failed – give up  */
        return;

    /* copy old values if any  */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    cache     = new_cache;
    cache_cap = (size_t)(need + 1);
}

/*  initialise cache with the first four terms  */
static void init_cache(void)
{
    if (cache_len >= 4)   /* already initialised  */
        return;

    ensure_capacity(3);   /* indices 0..3  */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    /*  n = 2  (even)  -> (10^3 - 91) / 9  */
    cache[2] = (pow10((idx_t)3) - (seq_t)91) / (seq_t)9;
    /*  n = 3  (odd)   -> (10^4 - 100) / 9  */
    cache[3] = (pow10((idx_t)4) - (seq_t)100) / (seq_t)9;
    cache_len = 4;
}

/*  sequence function  */
seq_t a(idx_t n)
{
    if (n < 4) {
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)0;
        if (n == 2) return (pow10((idx_t)3) - (seq_t)91) / (seq_t)9;
        /* n == 3 */
        return (pow10((idx_t)4) - (seq_t)100) / (seq_t)9;
    }

    init_cache();

    if (n + 1 > cache_len) {
        /* extend cache up to n (inclusive) */
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 10*a(i-1) + a(i-2) - 10*a(i-3)  */
            cache[i] = (seq_t)10 * cache[i - 1] + cache[i - 2] - (seq_t)10 * cache[i - 3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
