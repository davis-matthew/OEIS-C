/*
 * A305447  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-2) +7*a(n-3) -6*a(n-4) -18*a(n-5)
 *            -26*a(n-6) +8*a(n-7) +27*a(n-8) +47*a(n-9) +16*a(n-10)
 *            -5*a(n-11) -3*a(n-12) -7*a(n-13) -20*a(n-14) -24*a(n-15)
 *            -5*a(n-16) -6*a(n-17) for n>18 with a(1)=1, a(2)=11, a(3)=7,
 *            a(4)=20, a(5)=44, a(6)=77, a(7)=181, a(8)=379, a(9)=849,
 *            a(10)=1799,...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple memoisation cache and the recurrence
   coefficients above.  It relies on the utility function mul_small
   defined in "../utils/mul_small.h". */
#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;   /* cache[0] holds a(1) */
static idx_t cache_len = 0;   /* number of terms currently cached */
static size_t cache_cap = 0;  /* allocated capacity of cache */

seq_t a(idx_t n) {
    /* Initialise the cache on the first call with the first 18 values. */
    if (cache_len == 0) {
        cache_cap = (size_t)18;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Initial terms from OEIS A305447. */
        static const seq_t init[18] = {
            (seq_t)1,     (seq_t)11,   (seq_t)7,    (seq_t)20,
            (seq_t)44,    (seq_t)77,   (seq_t)181,  (seq_t)379,
            (seq_t)849,   (seq_t)1799, (seq_t)4154, (seq_t)9242,
            (seq_t)20384, (seq_t)46987,(seq_t)105477,(seq_t)238364,
            (seq_t)544013,(seq_t)1234035
        };
        for (int i = 0; i < 18; ++i)
            cache[i] = init[i];
        cache_len = 18;
    }

    /* Return cached value if already known. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache to hold terms up to n. */
    if ((size_t)n > cache_cap) {
        cache_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute values from cache_len+1 up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* idx_t i represents current n.  Use 1‑based indices. */
        /* Positive coefficient terms */
        seq_t pos = 0;
        pos += cache[i - 2];                          /* +1*a(i-1) */
        pos += mul_small(cache[i - 3], 3);            /* +3*a(i-2) */
        pos += mul_small(cache[i - 4], 7);            /* +7*a(i-3) */
        pos += mul_small(cache[i - 8], 8);            /* +8*a(i-7) */
        pos += mul_small(cache[i - 9], 27);           /* +27*a(i-8) */
        pos += mul_small(cache[i -10], 47);           /* +47*a(i-9) */
        pos += mul_small(cache[i -11], 16);           /* +16*a(i-10) */

        /* Negative coefficient terms */
        seq_t neg = 0;
        neg += mul_small(cache[i - 5], 6);            /* -6*a(i-4) */
        neg += mul_small(cache[i - 6], 18);           /* -18*a(i-5) */
        neg += mul_small(cache[i - 7], 26);           /* -26*a(i-6) */
        neg += mul_small(cache[i -12], 5);            /* -5*a(i-11) */
        neg += mul_small(cache[i -13], 3);            /* -3*a(i-12) */
        neg += mul_small(cache[i -14], 7);            /* -7*a(i-13) */
        neg += mul_small(cache[i -15], 20);           /* -20*a(i-14) */
        neg += mul_small(cache[i -16], 24);           /* -24*a(i-15) */
        neg += mul_small(cache[i -17], 5);            /* -5*a(i-16) */
        neg += mul_small(cache[i -18], 6);            /* -6*a(i-17) */

        seq_t val = pos;
        val -= neg;  /* pos >= neg for all valid n */

        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
