/*
 * A282185  Number of n X 2 0..2 arrays with no element equal to more than
 *          four of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A282185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) – 48*a(n-2) – 116*a(n-3) – 160*a(n-4) –
 *            96*a(n-5) – 36*a(n-6) for n > 9 Initial terms (n = 1..9):
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=8, a(5)=104, a(6)=1222, a(7)=13552,
 *            a(8)=144784, a(9)=1506870
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local static table with the first nine terms. */
    static const seq_t init[9] = {
        (seq_t)0,
        (seq_t)0,
        (seq_t)1,
        (seq_t)8,
        (seq_t)104,
        (seq_t)1222,
        (seq_t)13552,
        (seq_t)144784,
        (seq_t)1506870
    };

    /* Cached sequence values.  cache_len holds the number of terms
       currently stored (so cache[0] = a(1)). */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Trivial case: early terms. */
    if (n <= 9)
        return init[n - 1];

    /* Initialise cache on first use. */
    if (cache_len == 0) {
        cache_cap = 16;
        if (cache_cap < (size_t)n)
            cache_cap = (size_t)n;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* Grow cache if necessary. */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;   /* double until it fits */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];

            seq_t val = a1 * (seq_t)16;
            val -= a2 * (seq_t)48;
            val -= a3 * (seq_t)116;
            val -= a4 * (seq_t)160;
            val -= a5 * (seq_t)96;
            val -= a6 * (seq_t)36;

            cache[i] = val;
        }
        cache_len = (size_t)n;
    }

    /* Return the requested term (zero‑based indexing). */
    return cache[n - 1];
}
