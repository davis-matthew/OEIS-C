/*
 * A393409  Sum of the middle parts in the partitions of n into 3 parts whose
 *          largest part is even.
 * https://oeis.org/A393409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5)
          + 3*a(n-6) - 3*a(n-7) + 2*a(n-8) - 2*a(n-9) - 2*a(n-10)
          + 2*a(n-11) - 3*a(n-12) + 3*a(n-13) - a(n-14) + a(n-15)
          + a(n-16) - a(n-17) + a(n-18) - a(n-19)
   with initial values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=3,
   a(7)=2, a(8)=6, a(9)=9, a(10)=13, a(11)=13, a(12)=22, a(13)=24,
   a(14)=33, a(15)=35, a(16)=51, a(17)=56, a(18)=72 */

#include <stdlib.h>

static const seq_t init_vals[19] = {
    0, 0, 0, 0, 1, 2, 3, 2, 6, 9,
    13, 13, 22, 24, 33, 35, 51, 56, 72
};

static seq_t *cache = NULL;      /* cache[0..cache_len-1] already computed */
static size_t cache_len = 0;      /* number of elements stored */

/* Return a(n) for any n >= 0.  Results are memoised in <cache>. */
seq_t a(idx_t n)
{
    /* For n < 19 we use the initial list. */
    if (n < 19)
        return init_vals[(size_t)n];

    /* First call: initialise the cache with the 19 seed values. */
    if (!cache) {
        cache_len = 19;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* If n is already computed, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache if necessary.  We allocate enough space for all
       indices up to n, but we don't update cache_len until after
       computing the new values. */
    size_t required = (size_t)n + 1;   /* need indices 0..n inclusive */
    if (required > cache_len) {
        seq_t *newp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!newp) {               /* allocation failed – abort */
            return (seq_t)0;
        }
        cache = newp;
        /* cache_len stays unchanged – the new memory is uninitialised,
           but we will compute the missing terms below. */
    }

    /* Compute terms from the current length up to n, inclusive. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 0;

        val += cache[i - 1];
        val -= cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 4];
        val -= cache[i - 5];
        val += cache[i - 6] * (seq_t)3;
        val -= cache[i - 7] * (seq_t)3;
        val += cache[i - 8] * (seq_t)2;
        val -= cache[i - 9] * (seq_t)2;
        val -= cache[i -10] * (seq_t)2;
        val += cache[i -11] * (seq_t)2;
        val -= cache[i -12] * (seq_t)3;
        val += cache[i -13] * (seq_t)3;
        val -= cache[i -14];
        val += cache[i -15];
        val += cache[i -16];
        val -= cache[i -17];
        val += cache[i -18];
        val -= cache[i -19];

        cache[i] = val;
    }

    /* Update the computed length. */
    cache_len = required;

    return cache[(size_t)n];
}
