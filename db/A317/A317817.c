/*
 * A317817  Number of nX2 0..1 arrays with every element unequal to 1, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-4) - 2*a(n-5) - a(n-6) for n >
 *            6 Initial values: a(1)=1, a(2)=5, a(3)=16, a(4)=50, a(5)=160,
 *            a(6)=511
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Handle the first six terms directly */
    if (n <= 6) {
        static const seq_t base[7] = {
            (seq_t)0,
            (seq_t)1,
            (seq_t)5,
            (seq_t)16,
            (seq_t)50,
            (seq_t)160,
            (seq_t)511
        };
        return base[(size_t)n];
    }

    /* Cached values for arbitrary n */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed_max = 0;
    size_t idx = (size_t)n;

    /* Grow the cache if necessary */
    if (cap <= idx) {
        size_t newcap = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Initialize cache with known base values */
    if (computed_max == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)50;
        cache[5] = (seq_t)160;
        cache[6] = (seq_t)511;
        computed_max = 6;
    }

    /* Compute terms up to n using the recurrence */
    if (computed_max < idx) {
        for (size_t i = computed_max + 1; i <= idx; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)3 + cache[i - 2];
            seq_t t2 = cache[i - 4] * (seq_t)3
                     + cache[i - 5] * (seq_t)2
                     + cache[i - 6];
            cache[i] = t1 - t2;
        }
        computed_max = idx;
    }

    return cache[idx];
}
