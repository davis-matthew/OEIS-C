/*
 * A310007  Coordination sequence Gal.4.31.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5) * -
 *            7*a(n-6) + 8*a(n-7) - 7*a(n-8) + 6*a(n-9) - 5*a(n-10) * +
 *            4*a(n-11) - 3*a(n-12) + 2*a(n-13) - a(n-14) for n > 16 * *
 *            Initial values: * a(0)=1, a(1)=3, a(2)=4, a(3)=7, a(4)=12,
 *            a(5)=18, a(6)=19, a(7)=17, * a(8)=18, a(9)=23, a(10)=27,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;   /* cached values */
static idx_t cache_size = 0;  /* number of cached elements */
static size_t cache_cap = 0;  /* allocated capacity */

/* Forward declaration of the base values */
static const seq_t initial[17] = {
    1, 3, 4, 7, 12, 18, 19, 17, 18, 23, 27, 32, 40, 43, 40, 37, 38
};

seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 17;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (sequence not defined) */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache if empty */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 17; ++i)
            cache[i] = initial[i];
        cache_size = 17;
    }

    /* Return known value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Compute values up to n iteratively using the recurrence */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t val = 0;
        if (k >= 1)  val +=  2 * cache[k - 1];
        if (k >= 2)  val -=  3 * cache[k - 2];
        if (k >= 3)  val +=  4 * cache[k - 3];
        if (k >= 4)  val -=  5 * cache[k - 4];
        if (k >= 5)  val +=  6 * cache[k - 5];
        if (k >= 6)  val -=  7 * cache[k - 6];
        if (k >= 7)  val +=  8 * cache[k - 7];
        if (k >= 8)  val -=  7 * cache[k - 8];
        if (k >= 9)  val +=  6 * cache[k - 9];
        if (k >= 10) val -=  5 * cache[k - 10];
        if (k >= 11) val +=  4 * cache[k - 11];
        if (k >= 12) val -=  3 * cache[k - 12];
        if (k >= 13) val +=  2 * cache[k - 13];
        if (k >= 14) val -=  1 * cache[k - 14];
        cache[k] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
